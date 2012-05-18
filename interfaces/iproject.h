/*
 * iproject.h
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef IPROJECT_H_
#define IPROJECT_H_

#include "simpleeventinfo.h"

class IProject
{
public:
    virtual SimpleEventInfo *info(int &size) = 0;
    virtual ~IProject() { }
};

#endif /* IPROJECT_H_ */
