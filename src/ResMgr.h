/*
 * lftp and utils
 *
 * Copyright (c) 1996-1997 by Alexander V. Lukyanov (lav@yars.free.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* $Id$ */

#ifndef RESMGR_H
#define RESMGR_H

#include <stdio.h>
#include "xmalloc.h"

typedef const char *ResValValid(char **value);
typedef const char *ResClValid(char **closure);

class ResMgr
{
   friend class ResDecl;

   class Resource
   {
      friend class ResMgr;

      const ResDecl *type;
      char *value;
      char *closure;

      Resource *next;

      bool ClosureMatch(const char *cl_data);

      Resource(Resource *next,const ResDecl *type,
	       char *closure,char *value)
      {
	 this->type=type;
	 this->value=value;
	 this->closure=closure;
	 this->next=next;
      }
      ~Resource()
      {
	 xfree(closure);
	 xfree(value);
      }
   };

   static Resource *chain;
   static ResDecl *type_chain;

   static int ResourceCompare(const void *a,const void *b);

public:
   static const char *Query(const char *name,const char *closure);

   enum CmpRes { EXACT,SUBSTR,DIFFERENT };

   static CmpRes VarNameCmp(const char *name1,const char *name2);
   static const char *FindVar(const char *name,ResDecl **type);
   static const char *Set(const char *name,const char *closure,const char *value);

   static void Print(FILE *);

   static const char *BoolValidate(char **value);
   static const char *NumberValidate(char **value);
   static const char *UNumberValidate(char **value);
   static bool str2bool(const char *value);
};

class ResDecl
{
   friend class ResMgr;

   ResDecl *next;
   ResValValid *val_valid;
   ResClValid *closure_valid;
public:
   const char *name;
   const char *defvalue;

   ResDecl(const char *name,const char *defvalue,
	   ResValValid *val_valid,ResClValid *closure_valid)
   {
      this->name=name;
      this->defvalue=defvalue;
      this->val_valid=val_valid;
      this->closure_valid=closure_valid;
      this->next=ResMgr::type_chain;
      ResMgr::type_chain=this;
      if(defvalue)
	 ResMgr::Set(name,0,defvalue);
   }

   const char *Query(const char *closure);
};

#endif //RESMGR_H
