#ifndef BASE_GAME_ENTITY_H
#define BASE_GAME_ENTITY_H

#include <vector>
#include <string>
#include "2D/Vector2D.h"
#include "2D/Geometry.h"
#include "misc/Utils.h"

struct Telegram;


class BaseGameEntity
{
public:
  
  enum {default_entity_type = -5};

  //used by the constructor to give each entity a unique ID
  //int NextValidID(){static int NextID = 0; return NextID++;}
private:
  
  //each entity has a unique ID
  int         m_ID;

  //every entity has a type associated with it (health, troll, ammo etc)
  int         m_EntityType;

  //this is a generic flag. 
  bool        m_bTag;
  
  //this is the next valid ID. Each time a BaseGameEntity is instantiated
  //this value is updated
  static int  m_iNextValidID;

  //this must be called within each constructor to make sure the ID is set
  //correctly. It verifies that the value passed to the method is greater
  //or equal to the next valid ID, before setting the ID and incrementing
  //the next valid ID
  void SetID(int val);

protected:
  
  //its location in the environment
  Vector2D m_vPos;

  Vector2D m_vScale;

  //the length of this object's bounding radius
  double    m_dBoundingRadius;

  
  BaseGameEntity():m_ID(GetNextValidID()),
                   m_dBoundingRadius(0.0),
                   m_vPos(Vector2D()),
                   m_vScale(Vector2D(1.0,1.0)),
                   m_EntityType(default_entity_type),
                   m_bTag(false)
  {}

  /*BaseGameEntity(int entity_type):m_ID(GetNextValidID()),
                   m_dBoundingRadius(0.0),
                   m_vPos(Vector2D()),
                   m_vScale(Vector2D(1.0,1.0)),
                   m_EntityType(entity_type),
                   m_bTag(false)
  {}*/
  
  BaseGameEntity(int entity_type, Vector2D pos, double r):m_vPos(pos),
                                        m_dBoundingRadius(r),
                                        m_ID(GetNextValidID()),
                                        m_vScale(Vector2D(1.0,1.0)),
                                        m_EntityType(entity_type),
                                        m_bTag(false)
                                        
  {}

  //this can be used to create an entity with a 'forced' ID. It can be used
  //when a previously created entity has been removed and deleted from the
  //game for some reason. For example, The Raven map editor uses this ctor 
  //in its undo/redo operations. 
  //USE WITH CAUTION!
  BaseGameEntity(int entity_type, int ForcedID):m_ID(ForcedID),
                   m_dBoundingRadius(0.0),
                   m_vPos(Vector2D()),
                   m_vScale(Vector2D(1.0,1.0)),
                   m_EntityType(entity_type),
                   m_bTag(false)
  {}

 BaseGameEntity(int ID);

public:

  virtual ~BaseGameEntity(){}

  virtual void Update(double time_elapsed){}; 

  virtual void Render(){};

  virtual bool HandleMessage(const Telegram& msg){return false;}
  
  //entities should be able to read/write their data to a stream
  virtual void Write(std::ostream&  os)const{}
  virtual void Read (std::ifstream& is){}
  
  //use this to grab the next valid ID
  static int   GetNextValidID(){return m_iNextValidID;}
  
  //this can be used to reset the next ID
  static void  ResetNextValidID(){m_iNextValidID = 0;}

  Vector2D     Pos()const{return m_vPos;}
  void         SetPos(Vector2D new_pos){m_vPos = new_pos;}

  double       BRadius()const{return m_dBoundingRadius;}
  void         SetBRadius(double r){m_dBoundingRadius = r;}
  int          ID()const{return m_ID;}

  bool         IsTagged()const{return m_bTag;}
  void         Tag(){m_bTag = true;}
  void         UnTag(){m_bTag = false;}

  Vector2D     Scale()const{return m_vScale;}
  void         SetScale(Vector2D val){m_dBoundingRadius *= MaxOf(val.x, val.y)/MaxOf(m_vScale.x, m_vScale.y); m_vScale = val;}
  void         SetScale(double val){m_dBoundingRadius *= (val/MaxOf(m_vScale.x, m_vScale.y)); m_vScale = Vector2D(val, val);} 

  int          EntityType()const{return m_EntityType;}
  void         SetEntityType(int new_type){m_EntityType = new_type;}

};



      
#endif




