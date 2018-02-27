
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
 *  Contact for this Plugin: 
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


#ifndef H__OGF_MMGIG_COMMANDS_MESH_GROBMMGCALLS_COMMANDS__H
#define H__OGF_MMGIG_COMMANDS_MESH_GROBMMGCALLS_COMMANDS__H

#include <OGF/mmgig/common/common.h>
#include <OGF/mesh/commands/mesh_grob_commands.h>
#include <OGF/mmgig/algo/mmg_wrapper.h>

namespace OGF {

    gom_class mmgig_API MeshGrobmmgcallsCommands : public MeshGrobCommands {
        public:
            MeshGrobmmgcallsCommands() ;
            virtual ~MeshGrobmmgcallsCommands() ;

gom_slots:

            /**
             * \menu /Tests
             */
            void run_mmg3d_tests() ;

            /**
             * \menu /MmgTools
             */
            void mmgs_remesh(
                    const std::string& output_name = "default_mmgs",
                    bool angle_detection = true,
                    double angle_value = 45.,
                    double hausd_bbox = 0.01,
                    double hsiz_bbox = 0.05, /* using hmin and hmax if set to 0 */
                    double hmin_bbox = 0.01,
                    double hmax_bbox = 0.2,
                    double hgrad = 1.105171,
                    bool enable_anisotropy = false,
                    bool noinsert = false,
                    bool noswap = false,
                    bool nomove = false,
                    const std::string& metric_attribute = "no_metric");

            /**
             * \menu /MmgTools
             */
            void mmg3d_remesh(
                    const std::string& output_name = "default_mmg3d",
                    bool angle_detection = true,
                    double angle_value = 45.,
                    double hausd_bbox = 0.01,
                    double hsiz_bbox = 0.05, /* using hmin and hmax if set to 0 */
                    double hmin_bbox = 0.01,
                    double hmax_bbox = 0.2,
                    double hgrad = 1.105171,
                    bool enable_anisotropy = false,
                    bool optim = false,
                    bool optimLES = false,
                    bool opnbdy = false,
                    bool noinsert = false,
                    bool noswap = false,
                    bool nomove = false,
                    bool nosurf = false,
                    const std::string& metric_attribute = "no_metric");
    } ;
}

#endif

