//--------------------------------------------------------------------------------------
//  This file is part of the Lomse library.
//  Copyright (c) 2010 Lomse project
//
//  Lomse is free software; you can redistribute it and/or modify it under the
//  terms of the GNU General Public License as published by the Free Software Foundation,
//  either version 3 of the License, or (at your option) any later version.
//
//  Lomse is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
//  PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with Lomse; if not, see <http://www.gnu.org/licenses/>.
//  
//  
//
//  For any comment, suggestion or feature request, please contact the manager of
//  the project at cecilios@users.sourceforge.net
//
//-------------------------------------------------------------------------------------

#include <lomse_time.h>

#include <cmath>

namespace lomse
{

//helper functions to compare times (two floating point numbers)

bool is_equal_time(float t1, float t2)
{
    return (fabs(t1 - t2) < 0.1f);
}

bool is_lower_time(float t1, float t2)
{
    return (t1 < t2) && (fabs(t2 - t1) >= 0.1f);
}

bool is_greater_time(float t1, float t2)
{
    return (t1 > t2) && (fabs(t1 - t2) >= 0.1f);
}


}   //namespace lomse
