/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2018 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 2013 Wayne Stambaugh <stambaughw@verizon.net>
 * Copyright (C) 1992-2018 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file class_edge_mod.h
 * @brief EDGE_MODULE class definition.
 */

#ifndef CLASS_EDGE_MOD_H_
#define CLASS_EDGE_MOD_H_


#include <wx/gdicmn.h>

#include <class_drawsegment.h>


class LINE_READER;
class MSG_PANEL_ITEM;


class EDGE_MODULE : public DRAWSEGMENT
{
public:
    EDGE_MODULE( MODULE* parent, STROKE_T aShape = S_SEGMENT );

    // Do not create a copy constructor & operator=.
    // The ones generated by the compiler are adequate.

    ~EDGE_MODULE();

    static inline bool ClassOf( const EDA_ITEM* aItem )
    {
        return aItem && PCB_MODULE_EDGE_T == aItem->Type();
    }

    /**
     * Move an edge of the footprint.
     * This is a footprint shape modification.
     * (should be only called by a footprint editing function)
     */
    void Move( const wxPoint& aMoveVector ) override;

    /**
     * Mirror an edge of the footprint.
     * Do not change the layer
     * This is a footprint shape modification.
     * (should be only called by a footprint editing function)
     */
    void Mirror( const wxPoint aCentre, bool aMirrorAroundXAxis );

    /**
     * Rotate an edge of the footprint.
     * This is a footprint shape modification.
     * (should be only called by a footprint editing function )
     */
    void Rotate( const wxPoint& aRotCentre, double aAngle ) override;

    /**
     * Flip entity relative to aCentre.
     * The item is mirrored, and layer changed to the paired corresponding layer if it is on
     * a paired layer.
     * This function should be called only from MODULE::Flip because it is not usual to flip
     * an item alone, without flipping the parent footprint (consider Mirror() instead).
     */
    void Flip( const wxPoint& aCentre, bool aFlipLeftRight ) override;

    bool IsParentFlipped() const;

    void SetStart0( const wxPoint& aPoint )     { m_Start0 = aPoint; }
    const wxPoint& GetStart0() const            { return m_Start0; }

    void SetEnd0( const wxPoint& aPoint )       { m_End0 = aPoint; }
    const wxPoint& GetEnd0() const              { return m_End0; }

    void SetBezier0_C1( const wxPoint& aPoint ) { m_Bezier0_C1 = aPoint; }
    const wxPoint& GetBezier0_C1() const        { return m_Bezier0_C1; }

    void SetBezier0_C2( const wxPoint& aPoint ) { m_Bezier0_C2 = aPoint; }
    const wxPoint& GetBezier0_C2() const        { return m_Bezier0_C2; }

    /**
     * Set relative coordinates from draw coordinates.
     * Call in only when the geometry or the footprint is modified and therefore the relative
     * coordinates have to be updated from the draw coordinates.
     */
    void SetLocalCoord();

    /**
     * Set draw coordinates (absolute values ) from relative coordinates.
     * Must be called when a relative coordinate has changed in order to see the changes on screen
     */
    void SetDrawCoord();

    void GetMsgPanelInfo( EDA_DRAW_FRAME* aFrame, std::vector<MSG_PANEL_ITEM>& aList ) override;

    wxString GetClass() const override
    {
        return wxT( "MGRAPHIC" );
    }

    wxString GetSelectMenuText( EDA_UNITS aUnits ) const override;

    BITMAP_DEF GetMenuImage() const override;

    EDA_ITEM* Clone() const override;

    virtual unsigned int ViewGetLOD( int aLayer, KIGFX::VIEW* aView ) const override;

#if defined(DEBUG)
    void Show( int nestLevel, std::ostream& os ) const override { ShowDummy( os ); }
#endif

    wxPoint m_Start0;       ///< Start point or center, relative to module origin, orient 0.
    wxPoint m_End0;         ///< End point, relative to module origin, orient 0.
    wxPoint m_Bezier0_C1;   ///< Bezier Control Point 1, relative to module origin, orient 0.
    wxPoint m_Bezier0_C2;   ///< Bezier Control Point 2, relative to module origin, orient 0.
};

#endif    // CLASS_EDGE_MOD_H_
