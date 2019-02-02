#include "GameManager.h"
int BULLET_BIT = 0x0001;
int WALL_BIT = 0x0002;
int FISH_BIT = 0x0004;

float PM_RATIO;
GameManager::GameManager(Setting *_setting)
{
    m_setting = _setting;
    c_listener = new ContactListener();
    c_listener->gameMgr = this;
    //_entityCollisionListener = NULL;
    PM_RATIO = m_setting->PM_RATIO;
	accumulator = 0.0f;
   
    initWorld();
}


GameManager::~GameManager()
{
    if(m_setting)
        delete m_setting;
    
}

void GameManager::initWorld()
{
    b2Vec2 gravity(m_setting->gravity.x,m_setting->gravity.y);
    m_world = new b2World(gravity);
    m_world->SetContactListener(c_listener);
    
	// bullet define
	bulletBodyDef.allowSleep = false;
	bulletBodyDef.bullet = false;
	bulletBodyDef.type = b2_dynamicBody;
	bulletBodyDef.fixedRotation = true;

	b2PolygonShape bulletShape; bulletShape.SetAsBox(20, 20);
	bulletFixtureDef.density = 1;
	bulletFixtureDef.shape = &bulletShape;
	bulletFixtureDef.isSensor = false;
    
    // fish body and fixture define
    fishBodyDef.allowSleep = false;
    fishBodyDef.type = b2_kinematicBody;
    
    
    // create Wall
    auto wall = new Wall();
    createWall(wall);
    
}

Entity* GameManager::createWall(Entity *m_wall)
{
    float wall_width = m_setting->world_size.x;
    float wall_height = m_setting->world_size.y;

	b2BodyDef wallBodyDef;
	wallBodyDef.position.Set(0, 0);

	m_wall->_body = m_world->CreateBody(&wallBodyDef);
    m_wall->_body->SetUserData(m_wall);

	b2EdgeShape wallShape;

	// bottom
	wallShape.Set(b2Vec2(0, 0), b2Vec2(wall_width, 0));
	b2Fixture *fix = m_wall->_body->CreateFixture(&wallShape, 0);
	fix->SetRestitution(1);
	b2Filter filter; filter.categoryBits = WALL_BIT;
	fix->SetFilterData(filter);

	// top
	wallShape.Set(b2Vec2(0, wall_height), b2Vec2(wall_width, wall_height));
	fix = m_wall->_body->CreateFixture(&wallShape, 0);
	fix->SetRestitution(1);
	fix->SetFilterData(filter);

	// left
	wallShape.Set(b2Vec2(0, 0), b2Vec2(0, wall_height));
	fix = m_wall->_body->CreateFixture(&wallShape, 0);
	fix->SetRestitution(1);
	fix->SetFilterData(filter);

	// right
	wallShape.Set(b2Vec2(wall_width, 0), b2Vec2(wall_width, wall_height));
	fix = m_wall->_body->CreateFixture(&wallShape, 0);
	fix->SetRestitution(1);
	fix->SetFilterData(filter);
    
    return m_wall;
}

void GameManager::update(float dt)
{
	if (dt >= 0.1)
		dt = 0.1;
	this->accumulator += dt;

	while (accumulator >= m_setting->FPS)
	{
		_doUpdate(m_setting->FPS);
		accumulator -= m_setting->FPS;
	}
}

void GameManager::_doUpdate(float dt)
{
    if(!m_world)
        return;
	for (vector<Entity *>::iterator iter = m_entities.begin(); iter != m_entities.end(); iter++)
	{
		(*iter)->update(dt);
        if((*iter)->need_remove && (*iter)->getType() == Entity::FISH )
        {
            b2Fixture* fix = (*iter)->_body->GetFixtureList();
            b2Filter newData;
            newData.categoryBits = 0;
            newData.maskBits = fix->GetFilterData().maskBits;
            fix->SetFilterData(newData);
        }
        
	}
	if (this->m_setting->using_constant_fps)
		m_world->Step(this->m_setting->FPS, 8, 3);
	else
		m_world->Step(dt, 8, 3);
    
    // remove entity
    int count = m_entities.size();
    while(count--)
    {
        if(m_entities[count]->need_remove)
        {
			if (m_entities[count]->getType() == Entity::FISH)
			{
				onRealDestroyFish(m_entities[count]);
			}
            if(m_entities[count]->_nodeDisplay)
            {
                m_entities[count]->_nodeDisplay->removeFromParent();
            }
            m_world->DestroyBody(m_entities[count]->_body);
            m_entities.erase(m_entities.begin() + count);
        }
    }
}

bool myfunction(Entity* i, Entity* j) { 
	if (i->_nodeDisplay && j->_nodeDisplay && (i->_nodeDisplay->getLocalZOrder() > j->_nodeDisplay->getLocalZOrder()))
		return true;
	return false;
}

Fish *GameManager::getFishByPos(Vec2 pos){
	Fish* ret = NULL;
	vector<Entity*> listFishCheck;
	for (int i = 0; i < m_entities.size(); i++){
		if (m_entities[i]->_type == Entity::FISH && m_entities[i]->_nodeDisplay){
			Node* node = m_entities[i]->_nodeDisplay;
			Vec2 convertPos = node->convertToNodeSpaceAR(pos);

			Size contentSize = m_entities[i]->contentSize;

			Rect rect_check = CCRectMake(-contentSize.width / 2, -contentSize.height / 2, contentSize.width, contentSize.height);
			if (rect_check.containsPoint(convertPos)){
				listFishCheck.push_back(m_entities[i]);
			}
		}
	}

	if (listFishCheck.size() > 0){
		std::sort(listFishCheck.begin(), listFishCheck.end(), myfunction);
		return (Fish *)listFishCheck[0];
	}
	return NULL;
}


Entity *GameManager::createBodyForBullet(Entity *bullet,Vec2 body_box)
{
    b2PolygonShape bulletShape; bulletShape.SetAsBox(body_box.x, body_box.y);
    bulletFixtureDef.density = 1 ;
    bulletFixtureDef.shape = &bulletShape;
    bulletFixtureDef.friction = 0;
    bulletFixtureDef.restitution = 1;
    
    bulletFixtureDef.isSensor = false;
    bulletFixtureDef.filter.categoryBits = BULLET_BIT;
    bulletFixtureDef.filter.maskBits = WALL_BIT | FISH_BIT;
    
    bullet->_body = m_world->CreateBody(&bulletBodyDef);
    bullet->_body->CreateFixture(&bulletFixtureDef);
    bullet->_body->SetUserData(bullet);
    
    m_entities.push_back(bullet);
    return bullet;
}

Entity *GameManager::createBodyForFish(Entity* fish,Vec2 body_box)
{
    b2PolygonShape fishShape;fishShape.SetAsBox(body_box.x, body_box.y);
    fishFixtureDef.density = 1;
    fishFixtureDef.shape = &fishShape;
    fishFixtureDef.isSensor = false;
    fishFixtureDef.filter.categoryBits = FISH_BIT;
    fishFixtureDef.filter.maskBits = BULLET_BIT;
    
    fish->_body = m_world->CreateBody(&fishBodyDef);
    fish->_body->CreateFixture(&fishFixtureDef);
    fish->_body->SetUserData(fish);
    m_entities.push_back(fish);
    return fish;
}

Entity* GameManager::shootBullet(Entity *bullet,Vec2 start_pos, Vec2 vel_dir)
{
    Vec2 vel = vel_dir;
    vel.normalize();
    vel = vel * m_setting->bullet_vel;
    
    bullet->setPosition(start_pos.x,start_pos.y);
	((Bullet *)bullet)->setVelLength(this->m_setting->bullet_vel);
    bullet->_body->SetLinearVelocity(b2Vec2(vel.x,vel.y));
    return bullet;
}


void GameManager::destroyEntity(Entity *entity)
{
    entity->need_remove = true;
}

void GameManager::onRealDestroyFish(Entity *entity)
{
	if (_fishDestroyDelegate != NULL)
	{
		_fishDestroyDelegate(entity);
	}
}

void GameManager::destroyAllEntity(bool removeNode){
	int count = m_entities.size();
	while (count--)
	{
		if (m_entities[count]->_nodeDisplay)
		{
			if (removeNode)
				m_entities[count]->_nodeDisplay->removeFromParent();
			else if (m_entities[count]->_nodeDisplay->getChildByTag(0))
			{
				m_entities[count]->_nodeDisplay->getChildByTag(0)->stopAllActions();
				if (m_entities[count]->_nodeDisplay->getChildByTag(1))
					m_entities[count]->_nodeDisplay->getChildByTag(1)->stopAllActions();

			}
		}
		m_world->DestroyBody(m_entities[count]->_body);
	}
	m_entities.clear();
}


void GameManager::BeginContact(b2Contact *contact)
{
    if(!contact->IsTouching())
        return;
    b2Fixture* fix1 = contact->GetFixtureA();
    b2Body *body1 = fix1->GetBody();
    b2Body *body2 = contact->GetFixtureB()->GetBody();
    
    if(body1->GetUserData() && body2->GetUserData())
    {
        Entity *entity1 = (Entity *)body1->GetUserData();
        Entity *entity2 = (Entity *)body2->GetUserData();
        if(entity1->getType() == Entity::FISH && entity2->getType() == Entity::BULLET)
        {
            if(_entityCollisionListener)
            {
                Vec2 pointCollide;pointCollide.x = -1;pointCollide.y = -1;
                
                if(contact->GetManifold()->pointCount > 0)
                {
                    b2WorldManifold manifold;
                    contact->GetWorldManifold(&manifold);
                    pointCollide.x = manifold.points[0].x;
                    pointCollide.y = manifold.points[0].y;
                }
                _entityCollisionListener(entity1, entity2,pointCollide);
            }
        }
        else if(entity2->getType() == Entity::FISH && entity1->getType() == Entity::BULLET)
        {
            if(_entityCollisionListener)
            {
                Vec2 pointCollide;pointCollide.x = -1;pointCollide.y = -1;
                
                if(contact->GetManifold()->pointCount > 0)
                {
                    b2WorldManifold manifold;
                    contact->GetWorldManifold(&manifold);
                    pointCollide.x = manifold.points[0].x;
                    pointCollide.y = manifold.points[0].y;
                }
                _entityCollisionListener(entity2, entity1,pointCollide);
            }
        }
		else if (entity2->getType() == Entity::WALL && entity1->getType() == Entity::BULLET)
		{
			((Bullet *)entity1)->live--;
			if (((Bullet *)entity1)->live < 0)
				destroyEntity(entity1);
		}
		else if (entity1->getType() == Entity::WALL && entity2->getType() == Entity::BULLET)
		{
			((Bullet *)entity2)->live--;
			if (((Bullet *)entity2)->live < 0)
				destroyEntity(entity2);
		}
        
        
    }
    
}

void GameManager::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
    if(!contact->IsTouching())
        return;
    
    b2Fixture* fix1 = contact->GetFixtureA();
    b2Body *body1 = fix1->GetBody();
    b2Body *body2 = contact->GetFixtureB()->GetBody();
    
    if(body1->GetUserData() && body2->GetUserData())
    {
        Entity *entity1 = (Entity *)body1->GetUserData();
        Entity *entity2 = (Entity *)body2->GetUserData();
        
//        bool enable_collision = true;
//        if(_contactPreSolve)
//        {
//            Vec2 pointCollide;pointCollide.x = -1;pointCollide.y = -1;
//
//            if(contact->GetManifold()->pointCount > 0)
//            {
//                b2WorldManifold manifold;
//                contact->GetWorldManifold(&manifold);
//                pointCollide.x = manifold.points[0].x;
//                pointCollide.y = manifold.points[0].y;
//            }
//            enable_collision = _contactPreSolve(entity1, entity2,pointCollide);
//        }
//
//        contact->SetEnabled(enable_collision);
        if( (entity1->getType() == Entity::FISH && entity2->getType() == Entity::BULLET ) ||
           (entity2->getType() == Entity::FISH && entity1->getType() == Entity::BULLET))
        {
            contact->SetEnabled(false);
        }
            
        
    }
}
