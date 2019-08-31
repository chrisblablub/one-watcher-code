/*#include <Engine/Math/Intersection.h>
#include <Engine/Utility/Frustum.h>

namespace env
{
	namespace intersection
	{
		INTERSECTION FrustumIntersectsAABB(const CFrustum& frustum, VECTOR3 vBMin, VECTOR3 vBMax)
		{
			bool bIntersetion = false;
			VECTOR3 vMin, vMax;
			for( int k = 0; k < 4; k++ )
			{
				// x
				if(frustum.GetFrustumClippingPlane(k).x >= 0 )
				{
					vMin.x = vBMin.x;
					vMax.x = vBMax.x;
				} else
				{
					vMin.x = vBMax.x;
					vMax.x = vBMin.x;
				}
				// y
				if( frustum.GetFrustumClippingPlane(k).y >= 0 )
				{
					vMin.y = vBMin.y;
					vMax.y = vBMax.y;
				} else
				{
					vMin.y = vBMax.y;
					vMax.y = vBMin.y;
				}
				// z
				if( frustum.GetFrustumClippingPlane(k).z >= 0 )
				{
					vMin.z = vBMin.z;
					vMax.z = vBMax.z;
				} else
				{
					vMin.z = vBMax.z;
					vMax.z = vBMin.z;
				}

				VECTOR3 vNormal = VECTOR3(frustum.GetFrustumClippingPlane(k).x, 
					frustum.GetFrustumClippingPlane(k).y, 
					frustum.GetFrustumClippingPlane(k).z);

				if(glm::dot(vNormal, vMin) + frustum.GetFrustumClippingPlane(k).w > 0)
					return INTERSECTION_OUTSIDE;

				if(glm::dot(vNormal, vMax) + frustum.GetFrustumClippingPlane(k).w >= 0)
					bIntersetion = true;
			}

			if( bIntersetion )
				return INTERSECTION_INTERSECTING;
			else
				return INTERSECTION_INSIDE;
		}

		INTERSECTION FrustumIntersectsBB(const CFrustum& frustum, const CBoundingVolume& box, const MATRIX& mtxBBoxTransform)
		{
			VECTOR3 vRight = box.GetRight();
			VECTOR3 vUp = box.GetUp();
			VECTOR3 vForward = box.GetForward();
			VECTOR3 vHalfLengths = box.GetHalfLengths();
			VECTOR3 vOrigin = box.GetOrigin();

			VECTOR3 vPoints[8];
			int l = 0;
			for(int i = -1; i < 2; i += 2)
			{
				// right
				VECTOR3 v1 = vOrigin + (float)i * vRight * vHalfLengths[0];

				for(int j = -1; j < 2; j += 2)
				{
					// Up
					VECTOR3 v2 = v1 + (float)j * vUp * vHalfLengths[1];

					for(int k = -1; k < 2; k += 2, ++l)
					{
						VECTOR3 v3 = v2 + (float)k * vForward * vHalfLengths[2];
						// Forward
						vPoints[l] = v3;
					}
				}
			}

			for(int i = 0; i < 8; ++i)
			{
				vPoints[i] = VECTOR3(VECTOR4(vPoints[i], 1.0f) * mtxBBoxTransform);
			}


			INTERSECTION result = INTERSECTION_INSIDE;
			int pointsOut = 0,
				pointsIn = 0;

			for(int i=0; i < 4; i++) 
			{
				pointsOut=0;
				pointsIn=0;

				for (int k = 0; k < 8 && (pointsIn==0 || pointsIn==0); k++)
				{
					float distance = glm::dot(frustum.GetFrustumClippingPlane(i), VECTOR4(vPoints[k], 1.0f));
					
					if (distance < 0)
						pointsOut++;
					else
						pointsIn++;
				}
				//if all corners are out
				if(pointsIn == 0)
					return INTERSECTION_OUTSIDE;
				// if some corners are out and others are in	
				else if(pointsOut > 0)
					result = INTERSECTION_INTERSECTING;
			}

			return result;
		}

		bool FrustumIntersectsPoint(const CFrustum& frustum, const VECTOR3& vPoint)
		{
			for( int i = 0; i < 5; i++ )
			{
				if(glm::dot(frustum.GetFrustumClippingPlane(i), VECTOR4(vPoint, 1.0f)))
					return false;
			}

			return true;
		}

		bool FrustumIntersectsSphere(const CFrustum& frustum, const VECTOR3& vCenter, float radius)
		{
			INTERSECTION intersection = INTERSECTION_INSIDE;
			for(int i = 0; i < 5; ++i)
			{
				float distance = glm::dot(frustum.GetFrustumClippingPlane(i), VECTOR4(vCenter, 1.0f));

				if(distance < -radius)
					return false;
				else if(distance < radius)
					intersection = INTERSECTION_INTERSECTING;
			}

			return true;

		}

		bool RayIntersectsBB(const CBoundingVolume& bbox, const VECTOR3& vOrigin, const VECTOR3& vDirection, float* pDistance)
		{
			if(pDistance)
				*pDistance = 0.0f;

			float eps = std::pow(10.0f, FLT_MIN_10_EXP);
			float tmin = FLT_MIN;
			float tmax = FLT_MAX;
			VECTOR3 p = bbox.GetOrigin() - vOrigin;


			// x
			float halfLengths[3];
			halfLengths[0] = bbox.GetHalfLengths().x;
			halfLengths[1] = bbox.GetHalfLengths().y;
			halfLengths[2] = bbox.GetHalfLengths().z;

			VECTOR3 vOrientation[3];
			vOrientation[0] = bbox.GetRight();
			vOrientation[1] = bbox.GetUp();
			vOrientation[2] = bbox.GetForward();

			for(int i = 0; i < 3; ++i)
			{
				float e = glm::dot(vOrientation[i], p);
				float f = glm::dot(vOrientation[i], vDirection);
				if(std::abs(f) > eps)
				{
					float t1 = (e + halfLengths[i]) / f;
					float t2 = (e - halfLengths[i]) / f;
					if(t1 > t2)
						std::swap(t1, t2);
					if(t1 > tmin)
						tmin = t1;
					if(t2 < tmax) 
						tmax = t2;
					if(tmin > tmax) 
						return false;
					if(tmax < 0)
						return false;
				} 
				else if(-e - halfLengths[i] > 0 || -e + halfLengths[i] < 0)
					return false;
			}

			if(tmin > 0) 
			{
				if(pDistance)
					*pDistance = tmin;
				return true;
			}
			else
			{
				if(pDistance)
					*pDistance = tmax;
				return true;
			}
			return true;
		}
	} // intersection
} // env*/