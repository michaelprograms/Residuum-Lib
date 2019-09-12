// File:       /std/user/refs.c
// Mudlib:     Nightmare
// Purpose:    To store refs in (See _refs)
// Author:     Douglas Reay (Pallando @ TMI, Nightmare, etc)
// Date V1.0:  93-06-05

static mapping refs;

void set_refs( mapping a ) { refs = ( mapp( a ) ? a : ([]) ); }

mapping get_refs() { return ( refs ? copy(refs) : ([]) ); }

void set_ref( string reference, mixed value )
{
  if( !refs ) refs = ([]);
  if( !reference ) reference = "default";
  if( undefinedp( value ) )
    map_delete( refs, reference );
  else
    refs[reference] = value;
}

mixed get_ref( string reference )
{
  if( !refs ) refs = ([]);
  if( !reference ) reference = "default";
  return refs[reference];
}
