#include "aBehaviors.h"

#include <math.h>
#include "GL/glew.h"
#include "GL/glut.h"

// Base Behavior
///////////////////////////////////////////////////////////////////////////////
Behavior::Behavior()
{
}

Behavior::Behavior( char* name) 
{
	m_name = name;
	m_pTarget = NULL;
}

Behavior::Behavior( Behavior& orig) 
{
	m_name = orig.m_name;
	m_pTarget = NULL;
}

string& Behavior::GetName() 
{
    return m_name;
}

// Behaviors derived from Behavior
//----------------------------------------------------------------------------//
// Seek behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Seek returns a maximum velocity towards the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position


Seek::Seek( AJoint* target) 
{
	m_name = "seek";
	m_pTarget = target;

}

Seek::Seek( Seek& orig) 
{
	m_name = "seek";
	m_pTarget = orig.m_pTarget;
}


Seek::~Seek()
{
}

vec3 Seek::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired

	vec3 error = targetPos - actorPos; 
	error.Normalize();
	Vdesired = error * actor->gMaxSpeed;

	return Vdesired;
}


// Flee behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Flee calculates a a maximum velocity away from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position

Flee::Flee( AJoint* target) 
{
	m_name = "flee";
	m_pTarget = target;
}

Flee::Flee( Flee& orig) 
{
	m_name = "flee";
	m_pTarget = orig.m_pTarget;
}

Flee::~Flee()
{
}

vec3 Flee::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired
	vec3 error = targetPos - actorPos; 
	error.Normalize();
	Vdesired = -error * actor->gMaxSpeed;



	return Vdesired;

}

// Arrival behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// the actors distance from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Arrival strength is in BehavioralController::KArrival


Arrival::Arrival( AJoint* target) 
{
	m_name = "arrival";
	m_pTarget = target;
}

Arrival::Arrival( Arrival& orig) 
{
	m_name = "arrival";
	m_pTarget = orig.m_pTarget;
}

Arrival::~Arrival()
{
}

vec3 Arrival::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired
	vec3 error = targetPos - actorPos;
	Vdesired = actor->KArrival * error;




	return Vdesired;
}


// Departure behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// 1/(actor distance) from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Departure strength is in BehavioralController::KDeparture

Departure::Departure(AJoint* target) 
{
	m_name = "departure";
	m_pTarget = target;
}

Departure::Departure( Departure& orig) 
{
	m_name = "departure";
	m_pTarget = orig.m_pTarget;
}

Departure::~Departure()
{
}

vec3 Departure::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired

	vec3 error = targetPos - actorPos;
	Vdesired = actor->KDeparture*(-error) / error.Length();




	return Vdesired;
}


// Avoid behavior
///////////////////////////////////////////////////////////////////////////////
//  For the given the actor, return a desired velocity in world coordinates
//  If an actor is near an obstacle, avoid adds a normal response velocity to the 
//  the desired velocity vector computed using arrival
//  Agent bounding sphere radius is in BehavioralController::radius
//  Avoidance parameters are  BehavioralController::TAvoid and BehavioralController::KAvoid

Avoid::Avoid(AJoint* target, vector<Obstacle>* obstacles) 
{
	m_name = "avoid";
	m_pTarget = target;
	mObstacles = obstacles;
}

Avoid::Avoid( Avoid& orig) 
{
	m_name = "avoid";
	m_pTarget = orig.m_pTarget;
	mObstacles = orig.mObstacles;
}

Avoid::~Avoid()
{
}

vec3 Avoid::calcDesiredVel( BehaviorController* actor)
{

	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	m_actorPos = actor->getPosition();
	m_actorVel = actor->getVelocity();

	//TODO: add your code here
	vec3 Varrival(0, 0, 0);
	// Step 1. compute initial value for Vdesired = Varrival so agent moves toward target
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vector<Obstacle>& obstacleList = *mObstacles;
	vec3 error = targetPos - m_actorPos;
	Varrival = actor->KArrival * error;


	vec3 Vavoid(0, 0, 0);
	//TODO: add your code here to compute Vavoid 



	// Step 2. compute Lb
	//TODO: add your code here
	double Lb = actor->TAvoid * actor->getVelocity().Length();


	// Step 3. find closest obstacle 
	//TODO: add your code here
	Obstacle closest_obs = obstacleList[0];
	double min_distance = ((obstacleList[0].m_Center.getLocalTranslation()) - m_actorPos).Length();
	for (int i = 1; i < obstacleList.size(); i++) 
	{
		double distance = ((obstacleList[i].m_Center.getLocalTranslation()) - m_actorPos).Length();
		if (distance < min_distance)
		{
			min_distance = distance;
			closest_obs = obstacleList[i];
		}
	}
	m_obstaclePos = closest_obs.m_Center.getLocalTranslation();

	// Step 4. determine whether agent will collide with closest obstacle (only consider obstacles in front of agent)
	//TODO: add your code here
	vec3 d_world = closest_obs.m_Center.getLocalTranslation() - m_actorPos;
	mat3 local2world;
	local2world.FromEulerAngles(mat3::RotOrder::XYZ, actor->getOrientation());
	vec3 d_local = local2world.Inverse() * d_world;
	double dx = d_local[2], dy = d_local[0];
	double rB = actor->gAgentRadius, rO = closest_obs.m_Radius;
	vec3 V_avoid(0.0, 0.0, 0.0);
	if (abs(dx) > Lb + rB + rO) 
	{
		//no collision
	}
	else if (abs(dx) <= Lb + rB + rO) 
	{
		//potential collision
		if (abs(dy) > rB + rO)
		{
			//no collision
		}
		else if (abs(dy) <= rB + rO) 
		{
			//potential collision
			//
			vec3 n_avoid = -vec3(dy, 0., 0.);
			V_avoid = n_avoid * ((actor->KAvoid * (rB + rO - abs(dy))) / (rB + rO));
			V_avoid = local2world * V_avoid;
		}
	}
	Vdesired = Vdesired + V_avoid;
	// Step 5.  if potential collision detected, compute Vavoid and set Vdesired = Varrival + Vavoid
	//TODO: add your code here




	return Vdesired;
	
}

void Avoid::display( BehaviorController* actor)
{
	//  Draw Debug info
	vec3 angle = actor->getOrientation();
	vec3 vel = actor->getVelocity();
	vec3 dir = vec3(cos(angle[1]), 0, sin(angle[1]));
	vec3 probe = dir * (vel.Length()/BehaviorController::gMaxSpeed)*BehaviorController::TAvoid;
	
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_obstaclePos[0], m_obstaclePos[1], m_obstaclePos[2]);
	glColor3f(0, 1, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_actorPos[0] + probe[0], m_actorPos[1] + probe[1], m_actorPos[2] + probe[2]);
	glEnd();
}


// Wander Behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Wander returns a desired velocity vector whose direction changes at randomly from frame to frame
// Wander strength is in BehavioralController::KWander

Wander::Wander() 
{
	m_name = "wander";
	m_Wander = vec3(1.0, 0.0, 0.0);
}

Wander::Wander( Wander& orig) 
{
	m_name = "wander";
	m_Wander = orig.m_Wander;
}

Wander::~Wander()
{
}

vec3 Wander::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();

	// compute Vdesired = Vwander

	// Step. 1 find a random direction
	//TODO: add your code here
  
	double radius = rand() % 100 + 50;
	double angle;
	angle = rand() % 360;
	angle = double(angle) / 180.0 * M_PI;
	vec3 Vwander = vec3(radius * cos(angle), 0.0, radius * sin(angle));
	// Step2. scale it with a noise factor
	//TODO: add your code here
	angle = rand() % 360;
	angle = double(angle) / 180.0 * M_PI;
	vec3 r_noise = vec3(400.0 * cos(angle), 0.0, 400.0 * sin(angle));
	r_noise = actor->KNoise * r_noise / r_noise.Length();


	// Step3. change the current Vwander  to point to a random direction
	//TODO: add your code here
	Vwander = actor->KWander * (Vwander + r_noise) / (Vwander + r_noise).Length();





	// Step4. scale the new wander velocity vector and add it to the nominal velocity
	//TODO: add your code here
	vec3 V0 = actor->getVelocity();
	if (V0.Length() > Vwander.Length()) 
	{
		V0 = V0 / V0.Length() * Vwander.Length();
	}
	Vdesired = V0 + Vwander;


	return Vdesired;
}


// Alignment behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity vector in world coordinates
// Alignment returns the average velocity of all active agents in the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Alignment parameters are in BehavioralController::RNeighborhood and BehavioralController::KAlign


Alignment::Alignment(AJoint* target, vector<AActor>* agents) 
{
	m_name = "alignment";
	m_pAgentList = agents;
	m_pTarget = target;
}



Alignment::Alignment( Alignment& orig) 
{
	m_name = orig.m_name;
	m_pAgentList = orig.m_pAgentList;
	m_pTarget = orig.m_pTarget;

}

Alignment::~Alignment()
{
}

vec3 Alignment::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_pAgentList;
	

	// compute Vdesired 
	
	// Step 1. compute value of Vdesired for fist agent (i.e. m_AgentList[0]) using an arrival behavior so it moves towards the target
	 
	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	//TODO: add your code here

	if (actor == leader)
	{
		vec3 error = targetPos - actorPos;
		Vdesired = actor->KArrival * error;
	}



	// Step 2. if not first agent compute Valign as usual
	//TODO: add your code here
	else {
		double count = 0.0;
		double radius = actor->gKNeighborhood;
		for (int i = 0; i < agentList.size(); i++)
		{
			BehaviorController* ac = agentList[i].getBehaviorController();
			if (ac != actor) {
				if ((ac->getPosition() - actorPos).Length() <= radius)
				{
					count++;
					Vdesired += ac->getVelocity();
				}
			}
		}
		if (count != 0) {
			Vdesired = actor->KAlignment * Vdesired / count;
		}
	}

	
	return Vdesired;
}

// Separation behavior
///////////////////////////////////////////////////////////////////////////////
// For the given te actor, return a desired velocity vector in world coordinates
// Separation tries to maintain a constant distance between all agents
// within the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Separation settings are in BehavioralController::RNeighborhood and BehavioralController::KSeperate

 

Separation::Separation( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "separation";
	m_AgentList = agents;
	m_pTarget = target;
}

Separation::~Separation()
{
}

Separation::Separation( Separation& orig) 
{
	m_name = "separation";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

vec3 Separation::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vseparate
	// TODO: add your code here to compute Vdesired 
	
	double radius = actor->gKNeighborhood;
	for (int i = 0; i < agentList.size(); i++)
	{
		BehaviorController* ac = agentList[i].getBehaviorController();
		if (ac != actor)
		{
			Vdesired += actor->KSeparation * (actorPos - ac->getPosition()) / ((actorPos - ac->getPosition()).Length() * (actorPos - ac->getPosition()).Length());
		}
	}





	if (Vdesired.Length() < 5.0)
		Vdesired = 0.0;
	
	return Vdesired;
}


// Cohesion behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// Cohesion moves actors towards the center of the group of agents in the neighborhood
//  agents[i] gives the pointer to the ith agent in the environment
//  Cohesion parameters are in BehavioralController::RNeighborhood and BehavioralController::KCohesion


Cohesion::Cohesion( vector<AActor>* agents) 
{
	m_name = "cohesion";
	m_AgentList = agents;
}

Cohesion::Cohesion( Cohesion& orig) 
{
	m_name = "cohesion";
	m_AgentList = orig.m_AgentList;
}

Cohesion::~Cohesion()
{
}

vec3 Cohesion::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vcohesion
	// TODO: add your code here 

	double radius = actor->gKNeighborhood;
	vec3 center(0.0, 0.0, 0.0);
	double mass = 0.0;
	for (int i = 0; i < agentList.size(); i++) 
	{
		BehaviorController* ac = agentList[i].getBehaviorController();
		center += ac->gMass * ac->getPosition();
		mass += ac->gMass;
	}
	center /= mass;

	Vdesired = actor->KCohesion * (center - actor->getPosition());




	return Vdesired;
}

// Flocking behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector  in world coordinates
// Flocking combines separation, cohesion, and alignment behaviors
//  Utilize the Separation, Cohesion and Alignment behaviors to determine the desired velocity vector


Flocking::Flocking( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "flocking";
	m_AgentList = agents;
	m_pTarget = target;
}

Flocking::Flocking( Flocking& orig) 
{
	m_name = "flocking";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Flocking::~Flocking()
{
}

vec3 Flocking::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;

	// compute Vdesired = Vflocking
	// TODO: add your code here 
	//speration
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 Vseparation(0.0, 0.0, 0.0), Vcohesion(0.0, 0.0, 0.0), Valignment(0.0, 0.0, 0.0);
	double radius = actor->gKNeighborhood;
	for (int i = 0; i < agentList.size(); i++) 
	{
		BehaviorController* ac = agentList[i].getBehaviorController();
		if (ac != actor) 
		{
			Vseparation += actor->KSeparation * (actorPos - ac->getPosition()) / ((actorPos - ac->getPosition()).Length() * (actorPos - ac->getPosition()).Length());
		}
	}
	//cohesion
	vec3 center(0.0, 0.0, 0.0);
	double mass = 0.0;
	for (int i = 0; i < agentList.size(); i++)
	{
		BehaviorController* ac = agentList[i].getBehaviorController();
		center += ac->gMass * ac->getPosition();
		mass += ac->gMass;
	}
	center /= mass;

	Vcohesion = actor->KCohesion * (center - actor->getPosition());
	//alignment
	BehaviorController* leader = agentList[0].getBehaviorController();
	if (actor == leader)
	{
		vec3 error = targetPos - actorPos;
		Valignment = actor->KArrival * error;
	}
	else 
	{
		double count = 0.0;
		double radius = actor->gKNeighborhood;
		for (int i = 0; i < agentList.size(); i++)
		{
			BehaviorController* ac = agentList[i].getBehaviorController();
			if (ac != actor) {
				if ((ac->getPosition() - actorPos).Length() <= radius)
				{
					count++;
					if (ac == leader)
					{
						Valignment += 12.0 * ac->getVelocity();
						count += 12;
					}
					else
						Valignment += ac->getVelocity();
				}
			}
		}
		if (count != 0)
		{
			Valignment = actor->KAlignment * Valignment / count;
		}
	}
		Vdesired = 0.4 * Vseparation + 0.1 * Vcohesion + 0.6 * Valignment;
	return Vdesired;
}

//	Leader behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// If the agent is the leader, move towards the target; otherwise, 
// follow the leader at a set distance behind the leader without getting to close together
//  Utilize Separation and Arrival behaviors to determine the desired velocity vector
//  You need to find the leader, who is always agents[0]

Leader::Leader( AJoint* target, vector<AActor>* agents) 
{
	m_name = "leader";
	m_AgentList = agents;
	m_pTarget = target;
}

Leader::Leader( Leader& orig) 
{
	m_name = "leader";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Leader::~Leader()
{
}

vec3 Leader::calcDesiredVel( BehaviorController* actor)
{
	
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;

	// TODO: compute Vdesired  = Vleader
	// followers should stay directly behind leader at a distance of -200 along the local z-axis

	float CSeparation = 4.0;  float CArrival = 2.0;

	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	mat3 Rmat = leader->getGuide().getLocalRotation();  // is rotattion matrix of lead agent
	vec3 Varrival;
	if (actor == leader) 
	{
		vec3 targetPos = m_pTarget->getLocalTranslation();
		vec3 error = targetPos - actorPos;
		Varrival = actor->KArrival * error;
	}
	else 
	{
		vec3 follower_targetPos = leader->getPosition();
		follower_targetPos = Rmat.Inverse() * follower_targetPos + vec3(0.0, 0.0, -200.0);
		follower_targetPos = Rmat * follower_targetPos;
		vec3 error = follower_targetPos - actorPos;
		Varrival = actor->KArrival * error;
	}

	vec3 Vseparation(0.0, 0.0, 0.0);
	double radius = actor->gKNeighborhood;
	for (int i = 0; i < agentList.size(); i++) 
	{
		BehaviorController* ac = agentList[i].getBehaviorController();
		if (ac != actor) 
		{
			Vseparation += actor->KSeparation * (actorPos - ac->getPosition()) / ((actorPos - ac->getPosition()).Length() * (actorPos - ac->getPosition()).Length());
		}
	}
	Vdesired = CSeparation * Vseparation + CArrival * Varrival;


	return Vdesired;
}

///////////////////////////////////////////////////////////////////////////////

