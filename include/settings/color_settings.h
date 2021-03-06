/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2020 Jon Evans <jon@craftyjon.com>
 * Copyright (C) 2020 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _COLOR_SETTINGS_H
#define _COLOR_SETTINGS_H

#include <gal/color4d.h>
#include <settings/json_settings.h>

using KIGFX::COLOR4D;

/**
 * Color settings are a bit different than most of the settings objects in that there
 * can be more than one of them loaded at once.
 *
 * Each COLOR_SETTINGS object corresponds to a single color scheme file on disk.
 * The default color scheme is called "default" and will be created on first run.
 *
 * When users change color settings, they have the option of modifying the default scheme
 * or saving their changes to a new color scheme file.
 *
 * Each COLOR_SETTINGS defines all the settings used across all parts of KiCad, but it's not
 * necessary for the underlying theme file to contain all of them.  The color settings cascade,
 * so if a user chooses a non-default scheme for a certain application, and that non-default
 * scheme file is missing some color definitions, those will fall back to those from the "default"
 * scheme (which is either loaded from disk or created if missing)
 *
 * Each application (eeschema, gerbview, pcbnew) can have a different active color scheme selected.
 * The "child applications" (library editors) inherit from either eeschema or pcbnew.
 */

#include <settings/json_settings.h>
#include <settings/parameters.h>

/**
 * For specifying whether to retrieve colors from the "pcbnew" or "fpedit" set.
 * Can be removed once color themes exist.
 */
enum class COLOR_CONTEXT
{
    PCB,
    FOOTPRINT
};

class COLOR_SETTINGS : public JSON_SETTINGS
{
public:
    /**
     * m_Pallete stores a list of colors that are used, in order, when a list of colors needs to
     * be generated for an application.  For example, layer colors in GerbView, or default layer
     * colors in PcbNew.
     */
    std::vector<COLOR4D> m_Palette;

    explicit COLOR_SETTINGS( std::string aFilename = "user" );

    virtual ~COLOR_SETTINGS() {}

    bool MigrateFromLegacy( wxConfigBase* aCfg ) override;

    COLOR4D GetColor( int aLayer ) const;

    COLOR4D GetDefaultColor( int aLayer );

    void SetColor( int aLayer, COLOR4D aColor );

    // TODO(JE) remove once color themes exist
    void SetColorContext( COLOR_CONTEXT aContext = COLOR_CONTEXT::PCB )
    {
        m_color_context = aContext;
    }

    const wxString& GetName() const { return m_displayName; }
    void SetName( const wxString& aName ) { m_displayName = aName; }

    bool GetOverrideSchItemColors() const { return m_overrideSchItemColors; }
    void SetOverrideSchItemColors( bool aFlag ) { m_overrideSchItemColors = aFlag; }

private:
    wxString m_displayName;
    bool     m_overrideSchItemColors;

    /**
     * Map of all layer colors.
     * The key needs to be a valid layer ID, see layers_id_colors_and_visibility.h
     */
    std::unordered_map<int, COLOR4D> m_colors;

    std::unordered_map<int, COLOR4D> m_defaultColors;

    // TODO(JE) remove once color themes exist
    COLOR_CONTEXT m_color_context;
};

class COLOR_MAP_PARAM : public PARAM_BASE
{
public:
    COLOR_MAP_PARAM( const std::string& aJsonPath, int aMapKey, COLOR4D aDefault,
                     std::unordered_map<int, COLOR4D>* aMap, bool aReadOnly = false ) :
            PARAM_BASE( aJsonPath, aReadOnly ), m_key( aMapKey ), m_default( aDefault ),
            m_map( aMap )
    {}

    void Load( JSON_SETTINGS* aSettings ) const override
    {
        if( m_readOnly )
            return;

        COLOR4D val = m_default;

        if( OPT<COLOR4D> optval = aSettings->Get<COLOR4D>( m_path ) )
            val = *optval;

        ( *m_map )[ m_key ] = val;
    }

    void Store( JSON_SETTINGS* aSettings) const override
    {
        aSettings->Set<COLOR4D>( m_path, ( *m_map )[ m_key ] );
    }

    int GetKey() const
    {
        return m_key;
    }

    COLOR4D GetDefault() const
    {
        return m_default;
    }

    virtual void SetDefault() override
    {
        ( *m_map )[ m_key ] = m_default;
    }

private:
    int m_key;
    COLOR4D m_default;
    std::unordered_map<int, COLOR4D>* m_map;
};

#endif
