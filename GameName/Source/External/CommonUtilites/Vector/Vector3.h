#pragma once


namespace CommonUtilities
{
	template <class T>
	class Vector3
	{
	public:
		T x;
		T y;
		T z;
		//Creates a null-vector
		Vector3<T>();

		//Creates a vector (aX, aY, aZ)
		Vector3<T>(const T& aX, const T& aY, const T& aZ);

		//Copy constructor (compiler generated)
		Vector3<T>(const Vector3<T>& aVector) = default;

		//Assignment operator (compiler generated)
		Vector3<T>& operator=(const Vector3<T>& aVector3) = default;

		//Destructor (compiler generated)
		~Vector3<T>() = default;

		//Returns the squared length of the vector
		T LengthSqr() const;

		//Returns the length of the vector
		T Length() const;

		//Returns a normalized copy of this
		Vector3<T> GetNormalized() const;

		//Normalizes the vector
		void Normalize();

		//Returns the dot product of this and aVector
		T Dot(const Vector3<T>& aVector) const;

		//Returns the cross product of this and aVector
		Vector3<T> Cross(const Vector3<T>& aVector) const;
	};

	template<class T> inline Vector3<T>::Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	template<class T> inline Vector3<T>::Vector3(const T& aX, const T& aY, const T& aZ)
	{
		x = aX;
		y = aY;
		z = aZ;
	}

	template <class T> Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		//Returns the vector sum of aVector0 and aVector1
		Vector3<T> Temp;
		Temp.x = aVector0.x + aVector1.x;
		Temp.y = aVector0.y + aVector1.y;
		Temp.z = aVector0.z + aVector1.z;

		return Temp;
	}

	template <class T> Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		//Returns the vector difference of aVector0 and aVector1

		Vector3<T> Temp;

		Temp.x = aVector0.x - aVector1.x;
		Temp.y = aVector0.y - aVector1.y;
		Temp.z = aVector0.z - aVector1.z;

		return Temp;
	}

	template <class T> Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
	{
		//Returns the vector aVector multiplied by the scalar aScalar

		Vector3<T> Temp;
		Temp.x = aVector.x * aScalar;
		Temp.y = aVector.y * aScalar;
		Temp.z = aVector.z * aScalar;

		return Temp;
	}

	template <class T> Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector)
	{
		//Returns the vector aVector multiplied by the scalar aScalar

		Vector3<T> Temp;
		Temp.x = aScalar * aVector.x;
		Temp.y = aScalar * aVector.y;
		Temp.z = aScalar * aVector.z;

		return Temp;
	}

	template <class T> Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar)
	{
		//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)

		Vector3<T> Temp;
		Temp.x = aVector.x / aScalar;
		Temp.y = aVector.y / aScalar;
		Temp.z = aVector.z / aScalar;

		return Temp;
	}

	template <class T> void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		//Equivalent to setting aVector0 to (aVector0 + aVector1)

		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
	}

	template <class T> void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		//Equivalent to setting aVector0 to (aVector0 - aVector1)
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;

	}

	template <class T> void operator*=(Vector3<T>& aVector, const T& aScalar)
	{
		//Equivalent to setting aVector to (aVector * aScalar)
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
	}

	template <class T> void operator/=(Vector3<T>& aVector, const T& aScalar)
	{
		//Equivalent to setting aVector to (aVector / aScalar)
		aVector.x /= aScalar;
		aVector.y /= aScalar;
		aVector.z /= aScalar;
	}

	template <class T> bool operator!=(Vector3<T>& aVector0, Vector3<T>& aVector1)
	{
		return !(aVector0.x >= aVector1.x - 0.0001f && aVector0.x <= aVector1.x + 0.0001f &&
			aVector0.y >= aVector1.y - 0.0001f && aVector0.y <= aVector1.y + 0.0001f &&
			aVector0.z >= aVector1.z - 0.0001f && aVector0.z <= aVector1.z + 0.0001f);
	}

	template <class T> bool operator==(Vector3<T>& aVector0, Vector3<T>& aVector1)
	{
		return (aVector0.x >= aVector1.x - 0.0001f && aVector0.x <= aVector1.x + 0.0001f &&
			aVector0.y >= aVector1.y - 0.0001f && aVector0.y <= aVector1.y + 0.0001f &&
			aVector0.z >= aVector1.z - 0.0001f && aVector0.z <= aVector1.z + 0.0001f);
	}



	template<class T> inline T Vector3<T>::LengthSqr() const
	{
		//Returns the squared length of the vector

		return (x * x) + (y * y) + (z * z);
	}

	template<class T> inline T Vector3<T>::Length() const
	{
		//Returns the length of the vector

		return sqrt(LengthSqr());
	}

	template<class T> inline Vector3<T> Vector3<T>::GetNormalized() const
	{
		//Returns a normalized copy of this
		T length = Length();
		Vector3<T> Temp;

		if (x == 0 && y == 0 && z == 0)
		{
			Temp.x = 0;
			Temp.y = 0;
			Temp.z = 0;
			return Temp;
		}

		Temp.x = x;
		Temp.y = y;
		Temp.z = z;
		Temp.x *= (1 / length);
		Temp.y *= (1 / length);
		Temp.z *= (1 / length);

		return Temp;
	}

	template<class T> inline void Vector3<T>::Normalize()
	{
		if (x == 0 && y == 0 && z == 0)
			return;

		T length = Length();

		x *= (1 / length);
		y *= (1 / length);
		z *= (1 / length);
	}

	template<class T> inline T Vector3<T>::Dot(const Vector3<T>& aVector) const
	{
		//Returns the dot product of this and aVector

		T result = (x * aVector.x) + (y * aVector.y) + (z * aVector.z);

		return result;
	}

	template<class T> inline Vector3<T> Vector3<T>::Cross(const Vector3<T>& aVector) const
	{
		//Returns the cross product of this and aVector

		Vector3<T> Temp((y * aVector.z) - (z * aVector.y), (z * aVector.x) - (x * aVector.z), (x * aVector.y) - (y * aVector.x));

		return Temp;
	}

}