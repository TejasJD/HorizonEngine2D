namespace Hzn
{
    public struct Vector3
    {
        public float X, Y, Z;

        public static Vector3 Zero => new Vector3(0.0f);

        public Vector3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vector3(Vector2 xy, float z)
        {
            X = xy.X;
            Y = xy.Y;
            Z = z;
        }

        public Vector2 XY
        {
            get => new Vector2(X, Y);
            set
            {
                X = value.X;
                Y = value.Y;
            }
        }

        public override bool Equals(object obj)
        {
            if (obj.GetType() != GetType()) return false;
            Vector3 b = (Vector3)obj;
            return (X - b.X) < 1e-6f && (Y - b.Y) < 1e-6f && (Z - b.Z) < 1e-6f;
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }

        public static Vector3 operator *(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }

        public static bool operator ==(Vector3 a, Vector3 b)
        {
            return (a.X - b.X) < 1e-6f && (a.Y - b.Y) < 1e-6f && (a.Z - b.Z) < 1e-6f;
        }

        public static bool operator !=(Vector3 a, Vector3 b)
        {
            return !(a == b);
        }

    }
}