
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
 


#include <OGF/mmgig/commands/mesh_grobmmgcalls_commands.h>

#include <OGF/mmgig/algo/mmg_wrapper.h>

#include <geogram/mesh/mesh_geometry.h>

namespace OGF {

    MeshGrobmmgcallsCommands::MeshGrobmmgcallsCommands() { 
    }
        
    MeshGrobmmgcallsCommands::~MeshGrobmmgcallsCommands() { 
    }        

    void MeshGrobmmgcallsCommands::run_mmg3d_tests()  {
        MeshGrob* Mo = MeshGrob::find_or_create(scene_graph(), mesh_grob()->name() + "_g2m2g");
        mmg_wrapper_test_geo2mmg2geo(*mesh_grob(), *Mo); 
        Mo->update();
    }

    double scale_to_bbox(double value, const double xyzmin[3], const double xyzmax[3]) {
        double min_axis = DBL_MAX;
        for (index_t d = 0; d < 3; ++d) {
            if (xyzmax[d] - xyzmin[d] > 0 &&  xyzmax[d] - xyzmin[d] < min_axis) {
                min_axis = xyzmax[d] - xyzmin[d];
            }
        }
        geo_assert(min_axis != 0. && min_axis != DBL_MAX);
        return min_axis * value;
    }

    void MeshGrobmmgcallsCommands::mmgs_remesh(
            const std::string& output_name,
            bool angle_detection,
            double angle_value,
            double hausd_bbox,
            double hsiz_bbox,
            double hmin_bbox,
            double hmax_bbox,
            double hgrad,
            bool enable_anisotropy,
            bool noinsert,
            bool noswap,
            bool nomove,
            const std::string& metric_attribute
            ) {
        if (mesh_grob()->cells.nb() > 0 || mesh_grob()->facets.nb() == 0 || !mesh_grob()->facets.are_simplices()) {
            Logger::err("mmgs_remesh") << "input mesh should be a closed triangulated mesh, cancel" << std::endl;
            return;
        }
        std::string name = output_name;
        if (output_name == "default_mmg3d") {
            name = mesh_grob()->name() + "_mmg3d";
        }
        double xyzmin[3];
        double xyzmax[3];
        GEO::get_bbox(*mesh_grob(), xyzmin, xyzmax);
        MmgOptions opt;
        opt.angle_detection   = angle_detection;
        opt.angle_value       = angle_value;
        opt.hausd             = scale_to_bbox(hausd_bbox, xyzmin, xyzmax);
        opt.hsiz              = scale_to_bbox(hsiz_bbox , xyzmin, xyzmax);
        opt.hmin              = scale_to_bbox(hmin_bbox , xyzmin, xyzmax);
        opt.hmax              = scale_to_bbox(hmax_bbox , xyzmin, xyzmax);
        opt.hgrad             = hgrad;
        opt.enable_anisotropy = enable_anisotropy;
        opt.noinsert          = noinsert;
        opt.noswap            = noswap;
        opt.nomove            = nomove;
        opt.metric_attribute  = metric_attribute;
        MeshGrob* Mo = MeshGrob::find_or_create(scene_graph(), name);
        if (mmgs_tri_remesh(*mesh_grob(), *Mo, opt)) {
            Mo->update();
        } else {
            Mo->clear();
        }
        return;
    }

    void MeshGrobmmgcallsCommands::mmg3d_remesh(
            const std::string& output_name,
            bool angle_detection,
            double angle_value,
            double hausd_bbox,
            double hsiz_bbox,
            double hmin_bbox,
            double hmax_bbox,
            double hgrad,
            bool enable_anisotropy,
            bool optim,
            bool optimLES,
            bool opnbdy,
            bool noinsert,
            bool noswap,
            bool nomove,
            bool nosurf,
            const std::string& metric_attribute) {
        if (mesh_grob()->cells.nb() == 0 || !mesh_grob()->cells.are_simplices()) {
            Logger::err("mmg3d_remesh") << "input mesh should be a tetrahedral mesh, cancel" << std::endl;
            return;
        }
        std::string name = output_name;
        if (output_name == "default_mmg3d") {
            name = mesh_grob()->name() + "_mmg3d";
        }
        double xyzmin[3];
        double xyzmax[3];
        GEO::get_bbox(*mesh_grob(), xyzmin, xyzmax);
        MmgOptions opt;
        opt.angle_detection   = angle_detection;
        opt.angle_value       = angle_value;
        opt.hausd             = scale_to_bbox(hausd_bbox, xyzmin, xyzmax);
        opt.hsiz              = scale_to_bbox(hsiz_bbox , xyzmin, xyzmax);
        opt.hmin              = scale_to_bbox(hmin_bbox , xyzmin, xyzmax);
        opt.hmax              = scale_to_bbox(hmax_bbox , xyzmin, xyzmax);
        opt.hgrad             = hgrad;
        opt.enable_anisotropy = enable_anisotropy;
        opt.optim             = optim;
        opt.optimLES          = optimLES;
        opt.opnbdy            = opnbdy;
        opt.noinsert          = noinsert;
        opt.noswap            = noswap;
        opt.nomove            = nomove;
        opt.nosurf            = nosurf;
        opt.metric_attribute  = metric_attribute;
        MeshGrob* Mo = MeshGrob::find_or_create(scene_graph(), name);
        if (mmg3d_tet_remesh(*mesh_grob(), *Mo, opt)) {
            Mo->update();
        } else {
            Mo->clear();
        }
        return;
    }

}
