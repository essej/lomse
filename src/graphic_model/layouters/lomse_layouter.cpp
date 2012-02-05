//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-2012 Cecilio Salmeron. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice, this 
//      list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright notice, this
//      list of conditions and the following disclaimer in the documentation and/or
//      other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
// SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// For any comment, suggestion or feature request, please contact the manager of
// the project at cecilios@users.sourceforge.net
//---------------------------------------------------------------------------------------

#include "lomse_layouter.h"

#include "lomse_gm_basic.h"
#include "lomse_internal_model.h"
#include "lomse_content_layouter.h"
#include "lomse_score_layouter.h"
#include "lomse_box_content_layouter.h"

namespace lomse
{

//=======================================================================================
// Layouter implementation
//=======================================================================================
Layouter::Layouter(ImoContentObj* pItem, Layouter* pParent, GraphicModel* pGModel,
                   LibraryScope& libraryScope, ImoStyles* pStyles)
    : m_fIsLayouted(false)
    , m_pGModel(pGModel)
    , m_pParentLayouter(pParent)
    , m_libraryScope(libraryScope)
    , m_pStyles(pStyles)
    , m_pItemMainBox(NULL)
    , m_pItem(pItem)
{
}

//---------------------------------------------------------------------------------------
// constructor for DocumentLayouter
Layouter::Layouter(LibraryScope& libraryScope)
    : m_fIsLayouted(false)
    , m_pGModel(NULL)
    , m_pParentLayouter(NULL)
    , m_libraryScope(libraryScope)
    , m_pStyles(NULL)
    , m_pItemMainBox(NULL)
    , m_pItem(NULL)
{
}

//---------------------------------------------------------------------------------------
GmoBox* Layouter::start_new_page()
{
    GmoBox* pParentBox = m_pParentLayouter->start_new_page();

    m_pageCursor = m_pParentLayouter->get_cursor();
    m_availableWidth = m_pParentLayouter->get_available_width();
    m_availableHeight = m_pParentLayouter->get_available_height();

    create_main_box(pParentBox, m_pageCursor, m_availableWidth, m_availableHeight);

    return m_pItemMainBox;
}

//---------------------------------------------------------------------------------------
void Layouter::layout_item(ImoContentObj* pItem, GmoBox* pParentBox)
{
    m_pCurLayouter = create_layouter(pItem);

    m_pCurLayouter->prepare_to_start_layout();
    while (!m_pCurLayouter->is_item_layouted())
    {
        m_pCurLayouter->create_main_box(pParentBox, m_pageCursor,
                                        m_availableWidth, m_availableHeight);
        m_pCurLayouter->layout_in_box();
        m_pCurLayouter->set_box_height();

        if (!m_pCurLayouter->is_item_layouted())
        {
            pParentBox = start_new_page();
        }
    }
    m_pCurLayouter->add_end_margins();

    //update cursor and available space
    GmoBox* pChildBox = m_pCurLayouter->get_item_main_box();
    if (pChildBox)  //AWARE: NullLayouter does not create a box
    {
        m_pageCursor.y = pChildBox->get_bottom();
        m_availableHeight -= pChildBox->get_height();
    }

    if (!pItem->is_score())
        delete m_pCurLayouter;
}

//---------------------------------------------------------------------------------------
void Layouter::set_cursor_and_available_space(GmoBox* pItemMainBox)
{
    m_pageCursor.x = m_pItemMainBox->get_content_left();
    m_pageCursor.y = m_pItemMainBox->get_content_top();

    m_availableWidth = m_pItemMainBox->get_content_width();
    m_availableHeight = m_pItemMainBox->get_content_height();
}

//---------------------------------------------------------------------------------------
void Layouter::set_box_height()
{
    LUnits start = m_pItemMainBox->get_origin().y;
    m_pItemMainBox->set_height( m_pageCursor.y - start );
}

//---------------------------------------------------------------------------------------
void Layouter::add_end_margins()
{
    ImoStyle* pStyle = m_pItem->get_style();
    if (pStyle && m_pItemMainBox)   //NullLayouter doesn't have main box
    {
        LUnits space = pStyle->get_lunits_property(ImoStyle::k_margin_bottom)
                       + pStyle->get_lunits_property(ImoStyle::k_border_width_bottom)
                       + pStyle->get_lunits_property(ImoStyle::k_padding_bottom);
        m_pItemMainBox->set_height( m_pItemMainBox->get_height() + space );
        m_pageCursor.y += space;
    }
}

//---------------------------------------------------------------------------------------
Layouter* Layouter::create_layouter(ImoContentObj* pItem)
{
    Layouter* pLayouter = LayouterFactory::create_layouter(pItem, this, m_pStyles);
    if (pItem->is_score())
        save_score_layouter(pLayouter);
    return pLayouter;
}


//=======================================================================================
// LayouterFactory implementation
//=======================================================================================
Layouter* LayouterFactory::create_layouter(ImoContentObj* pItem, Layouter* pParent,
                                           ImoStyles* pStyles)
{
    GraphicModel* pGModel = pParent->get_graphic_model();
    LibraryScope& libraryScope = pParent->get_library_scope();

    switch (pItem->get_obj_type())
    {
//        case k_imo_control:
//            return LOMSE_NEW ControlLayouter(pItem, pParent, pGModel, libraryScope, pStyles);

        case k_imo_dynamic:
        case k_imo_content:
            return LOMSE_NEW ContentLayouter(pItem, pParent, pGModel, libraryScope, pStyles);

        case k_imo_list:
            return LOMSE_NEW ListLayouter(pItem, pParent, pGModel, libraryScope, pStyles);

        case k_imo_para:
        case k_imo_heading:
            return LOMSE_NEW ParagraphLayouter(pItem, pParent, pGModel, libraryScope, pStyles);

        case k_imo_listitem:
            return LOMSE_NEW ListItemLayouter(pItem, pParent, pGModel, libraryScope, pStyles);

        case k_imo_multicolumn:
            return LOMSE_NEW MultiColumnLayouter(pItem, pParent, pGModel, libraryScope, pStyles);

        case k_imo_score:
            return LOMSE_NEW ScoreLayouter(pItem, pParent, pGModel, libraryScope);

        case k_imo_score_player:
            return LOMSE_NEW ScorePlayerLayouter(pItem, pParent, pGModel, libraryScope, pStyles);

        default:
            return LOMSE_NEW NullLayouter(pItem, pParent, pGModel, libraryScope);
    }
}


}  //namespace lomse