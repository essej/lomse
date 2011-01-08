//-------------------------------------------------------------------------------------------
//  This file is part of the Lomse library.
//  Copyright (c) 2010-2011 Lomse project
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
//  For any comment, suggestion or feature request, please contact the manager of
//  the project at cecilios@users.sourceforge.net
//
//------------------------------------------------------------------------------------------

#ifndef __LOMSE_SCORE_ENUMS_H__        //to avoid nested includes
#define __LOMSE_SCORE_ENUMS_H__

namespace lomse
{

//---------------------------------------------------------------------------------------
// Spacing methods for rendering scores
//
//    Two basic methods:
//    1. Fixed: the spacing between notes is constant, independently of note duration.
//    2. Proportional: the spacing is adjusted so that note position is proportional
//       to time.
//
//    In the proportional method several alternatives are posible [NOT IMPLEMENTED]:
//    1. Proportional constant: the proportion factor between time and space is fixed.
//       Two alternative methods for fixing this factor:
//          a) Fixed: is given by the vaule of a parameter
//          b) ShortNote: is computed for the shorter note in the score
//    2. Proportional variable: the proportion factor is computed for each bar. Two
//       alternatives:
//          a) ShortNote: is computed for the shorter note in the bar
//          b) NumBars: Computed so taht the number of bars in the system is a
//             predefined number
//
enum ESpacingMethod
{
    k_spacing_fixed = 1,
    k_spacing_proportional,
};




}   //namespace lomse

#endif    // __LOMSE_SCORE_ENUMS_H__
