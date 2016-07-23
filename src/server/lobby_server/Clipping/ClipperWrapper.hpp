//
//  ClipperWrapper.hpp
//  TheDeadForest
//
//  Created by mac on 2016. 2. 11..
//
//

#pragma once

#include <vector>
#include "Vec2.h"
#include "Physics.hpp"
#include "clipper.hpp"

namespace realtrick
{    
    namespace clipping
    {
        
        inline void convertPolyToPath(ClipperLib::Path& path, const geometry::Polygon& poly)
        {
            for(int j = 0 ; j < poly.vertices.size() ; ++ j)
                path.push_back(ClipperLib::IntPoint((ClipperLib::cInt)poly.vertices[j].x, (ClipperLib::cInt)poly.vertices[j].y));
        }
        
        inline void convertRectToPath(ClipperLib::Path& path, const Rect& rect)
        {
            path.push_back(ClipperLib::IntPoint((ClipperLib::cInt)rect.origin.x, (ClipperLib::cInt)rect.origin.y));
            path.push_back(ClipperLib::IntPoint((ClipperLib::cInt)(rect.origin.x + rect.size.width), (ClipperLib::cInt)rect.origin.y));
            path.push_back(ClipperLib::IntPoint((ClipperLib::cInt)(rect.origin.x + rect.size.width), (ClipperLib::cInt)rect.origin.y + rect.size.height));
            path.push_back(ClipperLib::IntPoint((ClipperLib::cInt)rect.origin.x, (ClipperLib::cInt)rect.origin.y + rect.size.height));
        }
        
        inline void convertPathToPoly(geometry::Polygon& poly, const ClipperLib::Path& path)
        {
            for(int j = 0 ; j < path.size() ; ++ j)
                poly.pushVertex(Vec2((float)path[j].X, (float)path[j].Y));
        }
        
        inline std::vector<geometry::Polygon> getClippedPolygons(const std::vector<geometry::Polygon>& subjects, const geometry::Polygon& clipArea)
        {
            // subjects, solutions, clip
            ClipperLib::Paths subj(subjects.size());
            ClipperLib::Paths solution;
            ClipperLib::Path clip;
            
            // poly -> path
            for(int i = 0 ; i < subjects.size() ; ++ i)
                convertPolyToPath(subj[i], subjects[i]);
            convertPolyToPath(clip, clipArea);
            
            // execute
            ClipperLib::Clipper c;
            for(int i = 0 ; i < subj.size() ; ++ i)
                c.AddPath(subj[i], ClipperLib::ptSubject, true);
            c.AddPath(clip, ClipperLib::ptClip, true);
            c.Execute(ClipperLib::ctIntersection, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
            
            
            // path -> poly
            std::vector<geometry::Polygon> ret(solution.size());
            for(int i = 0 ; i < solution.size() ; ++ i)
                convertPathToPoly(ret[i], solution[i]);
            
            return ret;
        }
        
        inline std::vector<geometry::Polygon> getClippedPolygons(const geometry::Polygon& subject, const geometry::Polygon& clipArea)
        {
            // subjects, solutions, clip
            ClipperLib::Path subj;
            ClipperLib::Paths solution;
            ClipperLib::Path clip;
            
            // poly -> path
            convertPolyToPath(subj, subject);
            convertPolyToPath(clip, clipArea);
            
            // execute
            ClipperLib::Clipper c;
            c.AddPath(subj, ClipperLib::ptSubject, true);
            c.AddPath(clip, ClipperLib::ptClip, true);
            c.Execute(ClipperLib::ctIntersection, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
            
            
            // path -> poly
            std::vector<geometry::Polygon> ret(solution.size());
            for(int i = 0 ; i < solution.size() ; ++ i)
                convertPathToPoly(ret[i], solution[i]);
            
            return ret;
        }
        
        inline std::vector<geometry::Polygon> getClippedPolygons(const geometry::Polygon& subject, const Rect& clipRect)
        {
            // subjects, solutions, clip
            ClipperLib::Path subj;
            ClipperLib::Paths solution;
            ClipperLib::Path clip;
            
            // poly -> path
            convertPolyToPath(subj, subject);
            convertRectToPath(clip, clipRect);
            
            // execute
            ClipperLib::Clipper c;
            c.AddPath(subj, ClipperLib::ptSubject, true);
            c.AddPath(clip, ClipperLib::ptClip, true);
            c.Execute(ClipperLib::ctIntersection, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
            
            // path -> poly
            std::vector<geometry::Polygon> ret(solution.size());
            for(int i = 0 ; i < solution.size() ; ++ i)
                convertPathToPoly(ret[i], solution[i]);
            
            return ret;
        }
        
    } // namespace clipping
    
} // namespace realtrick














