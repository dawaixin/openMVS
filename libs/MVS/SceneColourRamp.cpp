/*
* SceneColourRamp.cpp
*
* Author(s):
*
*      Raphaël Durand-Delacre <duranddelacre.raphael@gmail.com>
*
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*
* Additional Terms:
*
*      You are required to preserve legal notices and author attributions in
*      that material or in the Appropriate Legal Notices displayed by works
*      containing it.
*/

#include "Common.h"
#include "Scene.h"
#include <limits>

using namespace MVS;

// D E F I N E S ///////////////////////////////////////////////////

// uncomment to enable multi-threading based on OpenMP
#ifdef _USE_OPENMP
#define DENSE_USE_OPENMP
#endif

bool Scene::ColourRampPointCloud()
{ 

	if (pointcloud.points.IsEmpty()) 
	{
		VERBOSE("ERROR: EMPTY POINT CLOUD!");
		return false;
	}
		
	TD_TIMER_STARTD();

	//initialise the min and max values to infinites and the current z-value to 0
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::min();
	float currentZ = 0.0f;

	//get the z-coordinate of the lowest and highest point
    FOREACH(i, pointcloud.points)
    {
        currentZ = pointcloud.points[i].z;
		if (currentZ < minZ) {
			minZ = currentZ;
		}
		if (currentZ > maxZ) {
			maxZ = currentZ;
		}
    }
    VERBOSE("Found min and max point z-coordinates.");


	//Calculate the difference between the highest and lowest point
	float maxDiffZ = maxZ - minZ;
	float currentDiffZ = 0.0f;
	unsigned char scaleValue = 0;
	unsigned char currentR = 0;
	unsigned char currentG = 0;

	unsigned long pointCloudSize = pointcloud.points.GetSize();
	
	//For each point, get the z-value and modify the corresponding colour accordingly
	FOREACH(i, pointcloud.points)
	{
		currentZ = pointcloud.points[i].z;
		//determine where the current difference in Z value
		currentDiffZ = maxZ - currentZ;
		//determine RGB value of point
		scaleValue = (unsigned char) (currentDiffZ * 255)/maxDiffZ; 
		currentR = scaleValue;
		currentG = 255 - scaleValue;
		//Assign the RGB value to the point
		pointcloud.colors[i].r = currentR;
		pointcloud.colors[i].g = currentG;
	}

	DEBUG_EXTRA("Point-cloud colour ramped: %u points (%s)", pointCloudSize, TD_TIMER_GET_FMT().c_str());
    return true;
}

bool Scene::ColourRampMesh()
{
    return true;
}