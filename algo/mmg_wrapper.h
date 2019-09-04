
/*
 *  OGF/Graphite: Geometry and Graphics Programming Library + Utilities
 *  Copyright (C) 2000-2015 INRIA - Project ALICE
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  If you modify this software, you should include a notice giving the
 *  name of the person performing the modification, the date of modification,
 *  and the reason for such modification.
 *
 *  Contact for Graphite: Bruno Levy - Bruno.Levy@inria.fr
 *  Contact for this Plugin: Maxence Reberol
 *
 *     Project ALICE
 *     LORIA, INRIA Lorraine, 
 *     Campus Scientifique, BP 239
 *     54506 VANDOEUVRE LES NANCY CEDEX 
 *     FRANCE
 *
 *  Note that the GNU General Public License does not permit incorporating
 *  the Software into proprietary programs. 
 *
 * As an exception to the GPL, Graphite can be linked with the following
 * (non-GPL) libraries:
 *     Qt, tetgen, SuperLU, WildMagic and CGAL
 */
 

#ifndef H__OGF_MMGIG_MMG_WRAPPER__H
#define H__OGF_MMGIG_MMG_WRAPPER__H

#include <OGF/mmgig/common/common.h>

namespace GEO {
    class Mesh;
}

namespace OGF {

    bool mmg_wrapper_test_geo2mmg2geo(const Mesh& M_in, Mesh& M_out);

    /* See MmgTools documentation for interpreation
     *  https://www.mmgtools.org/mmg-remesher-try-mmg/mmg-remesher-options
     */
    struct MmgOptions {
        /* Remeshing */
        bool angle_detection = true;
        double angle_value = 45.;
        double hausd = 0.01;
        double hsiz = 0.; /* using hmin and hmax if set to 0 */
        double hmin = 0.01;
        double hmax = 2.;
        double hgrad = 1.105171;
        bool enable_anisotropy = false;
        bool optim = false;
        bool optimLES = false;
        bool opnbdy = false;
        bool noinsert = false;
        bool noswap = false;
        bool nomove = false;
        bool nosurf = false;
        std::string metric_attribute = "no_metric";
        /* Level set extraction */
        bool level_set = false;
        std::string ls_attribute = "no_ls";
        double ls_value = 0.;
    };

    bool mmgs_tri_remesh(const Mesh& M,
                         Mesh& M_out,
                         const MmgOptions& opt,
                          const std::string& edge_attribute,
                          const std::string& facet_attribute);

    bool mmg3d_tet_remesh(const Mesh& M,
                          Mesh& M_out,
                          const MmgOptions& opt,
                          const std::string& edge_attribute,
                          const std::string& facet_attribute,
                          const std::string& cell_attribute);

    bool mmg3d_extract_iso(const Mesh& M, Mesh& M_out, const MmgOptions& opt);
        
}

#endif

