#include "mage_geometry.h"
#include "FrameBuffer.h"
#include "EngineROM.h"
#include "EnginePanic.h"

extern FrameBuffer *mage_canvas;

MageGeometry::MageGeometry(uint32_t address)
{
	size_t segmentLengthsSize;
	//skip over name:
	address += 32;
	//read typeId:
	EngineROM_Read(
		address,
		sizeof(typeId),
		(uint8_t *)&typeId,
		"Failed to load Geometry property 'typeId'"
	);
	address += sizeof(typeId);

	//read pointCount:
	EngineROM_Read(
		address,
		sizeof(pointCount),
		(uint8_t *)&pointCount,
		"Failed to load Geometry property 'pointCount'"
	);
	address += sizeof(pointCount);

	//read segmentCount:
	EngineROM_Read(
		address,
		sizeof(segmentCount),
		(uint8_t *)&segmentCount,
		"Failed to load Geometry property 'segmentCount'"
	);
	address += sizeof(segmentCount);

	address += 1; //padding

	//read pathLength:
	EngineROM_Read(
		address,
		sizeof(pathLength),
		(uint8_t *)&pathLength,
		"Failed to load Geometry property 'pathLength'"
	);
	pathLength = ROM_ENDIAN_F4_VALUE(pathLength);
	address += sizeof(pathLength);

	//generate appropriately sized point array:
	points = std::make_unique<Point[]>(pointCount);

	//fill array one point at a time:
	for(int i=0; i<pointCount; i++){
		uint16_t x;
		uint16_t y;
		//get x value:
		EngineROM_Read(
			address,
			sizeof(x),
			(uint8_t *)&x,
			"Failed to load Geometry property 'x'"
		);
		x = ROM_ENDIAN_U2_VALUE(x);
		address += sizeof(x);
		//get y value:
		EngineROM_Read(
			address,
			sizeof(y),
			(uint8_t *)&y,
			"Failed to load Geometry property 'x'"
		);
		y = ROM_ENDIAN_U2_VALUE(y);
		address += sizeof(y);
		//assign values:
		points[i].x = x;
		points[i].y = y;
	}

	//generate appropriately sized array:
	segmentLengths = std::make_unique<float[]>(segmentCount);
	segmentLengthsSize = sizeof(float) * segmentCount;

	EngineROM_Read(
		address,
		segmentLengthsSize,
		(uint8_t *)segmentLengths.get(),
		"Failed to load Geometry property 'x'"
	);
	ROM_ENDIAN_F4_BUFFER(segmentLengths.get(), segmentCount);

	return;
}

MageGeometry::MageGeometry(uint8_t type, uint8_t numPoints)
{
	typeId = type;
	pointCount = numPoints;
	points = std::make_unique<Point[]>(pointCount);
	for(uint8_t i=0; i<pointCount; i++)
	{
		points[i].x = 0;
		points[i].y = 0;
	}
}

uint32_t MageGeometry::size()
{
	uint32_t size =
		sizeof(typeId) +
		sizeof(pointCount) +
		sizeof(Point) * pointCount;
	return size;
}

bool MageGeometry::isPointInGeometry(Point point)
{
	//first check for the case where the geometry is a point:
	if(typeId == MageGeometryTypeId::POINT)
	{
		return (
			point.x == points[0].x &&
			point.y == points[0].y
		);
	}
	//if it's a polyline or polygon, do the thing:
	else if(
		typeId == MageGeometryTypeId::POLYLINE ||
		typeId == MageGeometryTypeId::POLYGON
	)
	{
		//refactoring stackoverflow code based on point-in-polygon by James Halliday
		//https://stackoverflow.com/questions/11716268/point-in-polygon-algorithm
		/*
		bool PointInPolygon(Point point, Polygon polygon) {
			vector<Point> points = polygon.getPoints();
			int i, j, nvert = points.size();
			bool c = false;

			for(i = 0, j = nvert - 1; i < nvert; j = i++) {
				if(( (points[i].y >= point.y ) != (points[j].y >= point.y) ) &&
					(point.x <= (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)
				)
				c = !c;
			}
			return c;
		}
		*/
		//Tim's version below:
		uint8_t i,j;
		bool c = false;
		for(i=0, j=pointCount - 1; i < pointCount; j = i++)
		{
			//get the points for i and j:
			Point points_i = points[i];
			Point points_j = points[j];
			//do the fancy check:
			if(
				( (points_i.y >= point.y) != (points_j.y >= point.y) ) &&
				( point.x <= (points_j.x - points_i.x) * (point.y - points_i.y) / (points_j.y - points_i.y) + points_i.x )
			)
			{ c = !c; }
		}
		//return the bool, that should be correct:
		return c;
	}
	//otherwise it's not a known geometry type, so always return false.
	else
	{
		return false;
	}
}


bool MageGeometry::doRectsOverlap(Rect a, Rect b)
{
	return !(
		a.x > (b.x + b.w) ||
		a.y > (b.y + b.h) ||
		b.x > (a.x + a.w) ||
		b.y > (a.y + a.h)
	);
}


void MageGeometry::draw(
	int32_t cameraX,
	int32_t cameraY,
	uint16_t color,
	int32_t offset_x,
	int32_t offset_y
)
{
	Point *pointA;
	Point *pointB;
	if(typeId == POINT) {
		mage_canvas->drawPoint(
			points[0].x + offset_x - cameraX,
			points[0].y + offset_y - cameraY,
			4,
			color
		);
	} else {
		for (int i = 1; i < pointCount; ++i) {
			pointA = &points[i - 1];
			pointB = &points[i];
			mage_canvas->drawLine(
				pointA->x + offset_x - cameraX,
				pointA->y + offset_y - cameraY,
				pointB->x + offset_x - cameraX,
				pointB->y + offset_y - cameraY,
				color
			);
		}
	}
	if(typeId == POLYGON) {
		// draw the closing line from point N-1 to 0
		pointA = &points[pointCount - 1];
		pointB = &points[0];
		mage_canvas->drawLine(
			pointA->x + offset_x - cameraX,
			pointA->y + offset_y - cameraY,
			pointB->x + offset_x - cameraX,
			pointB->y + offset_y - cameraY,
			color
		);
	}
}