
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
 

#include <OGF/mmgig/algo/mmg_wrapper.h>

#include <geogram/basic/logger.h>
#include <geogram/basic/attributes.h>
#include <geogram/mesh/mesh.h>

#include "mmg/libmmg.h"

namespace OGF {
    using namespace GEO;

    bool mmg_to_geo(const MMG5_pMesh mmg, Mesh& M) {
        printf("converting MMG5_pMesh to GEO::Mesh .. \n");
        /* Notes:
         * - indexing seems to start at 1 in MMG */

        geo_assert(mmg->dim == 3);
        M.clear();
        M.vertices.create_vertices((uint) mmg->np);
        M.edges.create_edges((uint) mmg->na);
        M.facets.create_triangles((uint) mmg->nt);
        M.cells.create_tets((uint) mmg->ne);

        for (uint v = 0; v < M.vertices.nb(); ++v) {
            for (uint d = 0; d < (uint) mmg->dim; ++d) {
                M.vertices.point_ptr(v)[d] = mmg->point[v+1].c[d];
            }
        }
        for (uint e = 0; e < M.edges.nb(); ++e) {
            M.edges.set_vertex(e,0,(uint) mmg->edge[e+1].a - 1);
            M.edges.set_vertex(e,1,(uint) mmg->edge[e+1].b - 1);
        }
        for (uint t = 0; t < M.facets.nb(); ++t) {
            M.facets.set_vertex(t,0,(uint) mmg->tria[t+1].v[0] - 1);
            M.facets.set_vertex(t,1,(uint) mmg->tria[t+1].v[1] - 1);
            M.facets.set_vertex(t,2,(uint) mmg->tria[t+1].v[2] - 1);
        }
        for (uint c = 0; c < M.cells.nb(); ++c) {
            M.cells.set_vertex(c,0,(uint) mmg->tetra[c+1].v[0] - 1);
            M.cells.set_vertex(c,1,(uint) mmg->tetra[c+1].v[1] - 1);
            M.cells.set_vertex(c,2,(uint) mmg->tetra[c+1].v[2] - 1);
            M.cells.set_vertex(c,3,(uint) mmg->tetra[c+1].v[3] - 1);
        }
        M.facets.connect();
        M.cells.connect();

        return true;
    }

    bool geo_to_mmg(const Mesh& M, MMG5_pMesh& mmg, MMG5_pSol& sol, bool volume_mesh = true) {
        printf("converting GEO::M to MMG5_pMesh .. \n");
        geo_assert(M.vertices.dimension() == 3);
        if (M.facets.nb() > 0) geo_assert(M.facets.are_simplices());
        if (M.cells.nb() > 0) geo_assert(M.cells.are_simplices());

        if (volume_mesh) {
            MMG3D_Init_mesh(MMG5_ARG_start, MMG5_ARG_ppMesh,&mmg,MMG5_ARG_ppMet,&sol, MMG5_ARG_end);
        } else {
            MMGS_Init_mesh(MMG5_ARG_start, MMG5_ARG_ppMesh,&mmg,MMG5_ARG_ppMet,&sol, MMG5_ARG_end);
        }

        if (volume_mesh && MMG3D_Set_meshSize(
                    mmg,
                    (int) M.vertices.nb(),
                    (int) M.cells.nb(),
                    0, /* nb prisms */
                    (int) M.facets.nb(),
                    0, /* nb quad */
                    (int) M.edges.nb()  /* nb edges */
                    ) != 1 ) {
            printf("failed to MMG3D_Set_meshSize\n");
            return false;
        } else if (!volume_mesh && MMGS_Set_meshSize(
                    mmg,
                    (int) M.vertices.nb(),
                    (int) M.facets.nb(),
                    (int) M.edges.nb()  /* nb edges */
                    ) != 1 ) {
            printf("failed to MMGS_Set_meshSize\n");
            return false;
        }

        for (uint v = 0; v < (uint) mmg->np; ++v) {
            for (uint d = 0; d < M.vertices.dimension(); ++d) {
                mmg->point[v+1].c[d] = M.vertices.point_ptr(v)[d];
            }
        }
        for (uint e = 0; e < (uint) mmg->na; ++e) {
            mmg->edge[e+1].a = (int) M.edges.vertex(e,0) + 1;
            mmg->edge[e+1].b = (int) M.edges.vertex(e,1) + 1;
        }
        for (uint t = 0; t < (uint) mmg->nt; ++t) {
            mmg->tria[t+1].v[0] = (int) M.facets.vertex(t,0) + 1;
            mmg->tria[t+1].v[1] = (int) M.facets.vertex(t,1) + 1;
            mmg->tria[t+1].v[2] = (int) M.facets.vertex(t,2) + 1;
        }
        if (volume_mesh) {
            for (uint c = 0; c < (uint) mmg->ne; ++c) {
                mmg->tetra[c+1].v[0] = (int) M.cells.vertex(c,0) + 1;
                mmg->tetra[c+1].v[1] = (int) M.cells.vertex(c,1) + 1;
                mmg->tetra[c+1].v[2] = (int) M.cells.vertex(c,2) + 1;
                mmg->tetra[c+1].v[3] = (int) M.cells.vertex(c,3) + 1;
            }
        }

        if (volume_mesh && MMG3D_Set_solSize(mmg,sol,MMG5_Vertex,(int)M.vertices.nb(),MMG5_Scalar) != 1 ) {
            printf("failed to MMG3D_Set_solSize\n");
            return false;
        } else if (!volume_mesh && MMGS_Set_solSize(mmg,sol,MMG5_Vertex,(int)M.vertices.nb(),MMG5_Scalar) != 1 ) {
            printf("failed to MMGS_Set_solSize\n");
            return false;
        }
        for(uint v = 0; v < M.vertices.nb(); ++v) {
            sol->m[v+1] = 1.;
        }
        if (volume_mesh && MMG3D_Chk_meshData(mmg,sol) != 1) {
            printf("error in mmg: inconsistant mesh and sol\n");
            return false;
        } else if (!volume_mesh && MMGS_Chk_meshData(mmg,sol) != 1) {
            printf("error in mmg: inconsistant mesh and sol\n");
            return false;
        }

        if (volume_mesh) {
            MMG3D_Set_handGivenMesh(mmg); /* because we don't use the API functions */
        } 

        return true;
    }

    void mmg3d_free(MMG5_pMesh mmg, MMG5_pSol sol){
        MMG3D_Free_all(MMG5_ARG_start,
                MMG5_ARG_ppMesh,&mmg,MMG5_ARG_ppMet,&sol, MMG5_ARG_end);
    }
    void mmgs_free(MMG5_pMesh mmg, MMG5_pSol sol){
        MMGS_Free_all(MMG5_ARG_start,
                MMG5_ARG_ppMesh,&mmg,MMG5_ARG_ppMet,&sol, MMG5_ARG_end);
    }

    bool mmg_wrapper_test_geo2mmg2geo(const Mesh& M_in, Mesh& M_out) {
        MMG5_pMesh mmg = NULL;
        MMG5_pSol sol = NULL;
        bool ok = geo_to_mmg(M_in, mmg, sol);
        if (!ok) return false;
        ok = mmg_to_geo(mmg, M_out);
        mmg3d_free(mmg, sol);
        return ok;
    }

    bool mmgs_tri_remesh(const Mesh& M, Mesh& M_out, const MmgOptions& opt) {
        MMG5_pMesh mesh = NULL;
        MMG5_pSol met = NULL;
        bool ok = geo_to_mmg(M, mesh, met, false);
        if (!ok) {
            Logger::err("mmgs_remesh") << "failed to convert mesh to MMG5_pMesh" << std::endl;
            mmgs_free(mesh, met);
            return false;
        }

        /* Set remeshing options */
        MMGS_Set_dparameter(mesh, met, MMGS_DPARAM_angleDetection, opt.angle_value);
        if (opt.enable_anisotropy) {
            MMGS_Set_solSize(mesh,met,MMG5_Vertex,0,MMG5_Tensor);
        }
        if (opt.hsiz == 0. || opt.metric_attribute != "no_metric") {
            MMGS_Set_dparameter(mesh, met, MMGS_DPARAM_hmin, opt.hmin);
            MMGS_Set_dparameter(mesh, met, MMGS_DPARAM_hmax, opt.hmax);
        } else {
            met->np = 0;
            MMGS_Set_dparameter(mesh, met, MMGS_DPARAM_hsiz, opt.hsiz);
        }
        MMGS_Set_dparameter(mesh, met, MMGS_DPARAM_hausd, opt.hausd);
        MMGS_Set_dparameter(mesh, met, MMGS_DPARAM_hgrad, opt.hgrad);
        MMGS_Set_iparameter(mesh, met, MMGS_IPARAM_angle, int(opt.angle_detection));
        MMGS_Set_iparameter(mesh, met, MMGS_IPARAM_noswap, int(opt.noswap));
        MMGS_Set_iparameter(mesh, met, MMGS_IPARAM_noinsert, int(opt.noinsert));
        MMGS_Set_iparameter(mesh, met, MMGS_IPARAM_nomove, int(opt.nomove));
        if (opt.metric_attribute != "no_metric") {
            if (!M.vertices.attributes().is_defined(opt.metric_attribute)) {
                Logger::err("mmgs_remesh") << opt.metric_attribute << " is not a vertex attribute, cancel" << std::endl;
                return false;
            }
            GEO::Attribute<double> h_local(M.vertices.attributes(), opt.metric_attribute);
            for(uint v = 0; v < M.vertices.nb(); ++v) {
                met->m[v+1] = h_local[v];
            }
        }

        int ier = MMGS_mmgslib(mesh,met);
        if (ier != MMG5_SUCCESS) {
            Logger::err("mmgs_remesh") << "failed to remesh" << std::endl;
            mmgs_free(mesh, met);
            return false;
        }

        ok = mmg_to_geo(mesh, M_out);

        mmgs_free(mesh, met);
        return ok;
    }


    bool mmg3d_tet_remesh(const Mesh& M, Mesh& M_out, const MmgOptions& opt) {
        MMG5_pMesh mesh = NULL;
        MMG5_pSol met = NULL;
        bool ok = geo_to_mmg(M, mesh, met, true);
        if (!ok) {
            Logger::err("mmg3d_remesh") << "failed to convert mesh to MMG5_pMesh" << std::endl;
            mmg3d_free(mesh, met);
            return false;
        }

        /* Set remeshing options */
        MMG3D_Set_dparameter(mesh, met, MMG3D_DPARAM_angleDetection, opt.angle_value);
        if (opt.enable_anisotropy) {
            MMG3D_Set_solSize(mesh,met,MMG5_Vertex,0,MMG5_Tensor);
        }
        if (opt.hsiz == 0. || opt.metric_attribute != "no_metric") {
            MMG3D_Set_dparameter(mesh, met, MMG3D_DPARAM_hmin, opt.hmin);
            MMG3D_Set_dparameter(mesh, met, MMG3D_DPARAM_hmax, opt.hmax);
        } else {
            met->np = 0;
            MMG3D_Set_dparameter(mesh, met, MMG3D_DPARAM_hsiz, opt.hsiz);
        }
        MMG3D_Set_dparameter(mesh, met, MMG3D_DPARAM_hausd, opt.hausd);
        MMG3D_Set_dparameter(mesh, met, MMG3D_DPARAM_hgrad, opt.hgrad);
        MMG3D_Set_iparameter(mesh, met, MMG3D_IPARAM_angle, int(opt.angle_detection));
        MMG3D_Set_iparameter(mesh, met, MMG3D_IPARAM_noswap, int(opt.noswap));
        MMG3D_Set_iparameter(mesh, met, MMG3D_IPARAM_noinsert, int(opt.noinsert));
        MMG3D_Set_iparameter(mesh, met, MMG3D_IPARAM_nomove, int(opt.nomove));
        MMG3D_Set_iparameter(mesh, met, MMG3D_IPARAM_nosurf, int(opt.nosurf));
        MMG3D_Set_iparameter(mesh, met, MMG3D_IPARAM_opnbdy, int(opt.opnbdy));
        MMG3D_Set_iparameter(mesh, met, MMG3D_IPARAM_optim, int(opt.optim));
        MMG3D_Set_iparameter(mesh, met, MMG3D_IPARAM_optimLES, int(opt.optimLES));
        if (opt.metric_attribute != "no_metric") {
            if (!M.vertices.attributes().is_defined(opt.metric_attribute)) {
                Logger::err("mmg3D_remesh") << opt.metric_attribute << " is not a vertex attribute, cancel" << std::endl;
                return false;
            }
            GEO::Attribute<double> h_local(M.vertices.attributes(), opt.metric_attribute);
            for(uint v = 0; v < M.vertices.nb(); ++v) {
                met->m[v+1] = h_local[v];
            }
        }

        int ier = MMG3D_mmg3dlib(mesh,met);
        if (ier != MMG5_SUCCESS) {
            Logger::err("mmg3d_remesh") << "failed to remesh" << std::endl;
            mmg3d_free(mesh, met);
            return false;
        }

        ok = mmg_to_geo(mesh, M_out);

        mmg3d_free(mesh, met);
        return ok;
    }
}
