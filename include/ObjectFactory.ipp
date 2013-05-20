/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

template< class BASE_CLASS > 
inline
ObjectFactory< BASE_CLASS >::~ObjectFactory() {
  destroyAll();
}

template< class BASE_CLASS > 
inline
void ObjectFactory< BASE_CLASS >::add( BASE_CLASS *obj, const string &name ) {

  objList.push_back( obj );
  
  if ( name != "" ) {
    objMap.insert( pair< const string, BASE_CLASS* >( name, obj ) );
  }
}

template< class BASE_CLASS > 
inline
void ObjectFactory< BASE_CLASS >::destroyAll() {
  typename list< BASE_CLASS* >::iterator 
    it = objList.begin(),
    endIt = objList.end();
  
  while ( it != endIt ) {
    delete *it;
    ++it;
  } 
  
  objList.clear();
  objMap.clear();
}

template< class BASE_CLASS > 
template< class OBJECT_CLASS > 
inline
OBJECT_CLASS* ObjectFactory< BASE_CLASS >::create( const string &name ) {
  OBJECT_CLASS *obj = new OBJECT_CLASS();
  add( obj, name );
  return obj;
}

template< class BASE_CLASS > 
inline
bool ObjectFactory< BASE_CLASS >::destroy( BASE_CLASS* obj ) {

  if ( obj == NULL ) {
    return false;
  }
  
  {
    typename list< BASE_CLASS* >::iterator it = find( objList.begin(), objList.end(), obj );
    
    if ( it != objList.end() ) {
      objList.erase( it );
    }
    else {
      return false;
    }
  }
  
  if ( obj->name() != "" ) {
    typename map< const string, BASE_CLASS* >::iterator it = objMap.find( obj->name() );
    
    if ( it != objMap.end() ) {
      objMap.erase( it );
    }
  }
  
  delete obj;
  return true;
}

template< class BASE_CLASS > 
inline
bool ObjectFactory< BASE_CLASS >::destroy( const string &name ) {
  
  if ( name == "" ) {
    return false;
  }
  
  BASE_CLASS *obj = NULL;
  
  {
    typename map< const string, BASE_CLASS* >::iterator it = objMap.find( name );
    
    if ( it != objMap.end() ) {
      obj = it->second;
      objMap.erase( it );
    }
  }
  
  if ( obj == NULL ) {
    return false;
  }
  
  {
    typename list< BASE_CLASS* >::iterator it = find( objList.begin(), objList.end(), obj );
    
    if ( it != objList.end() ) {
      objList.erase( it );
    }
  }
  
  delete obj;
  return true;
}

template< class BASE_CLASS > 
inline
BASE_CLASS* ObjectFactory< BASE_CLASS >::get( const string &name ) const {
  
  if ( name != "" ) {
    typename map< const string, BASE_CLASS* >::const_iterator it = objMap.find( name );
    
    if ( it != objMap.end() ) {
      return it->second;
    }
  }
  
  return NULL;
}
