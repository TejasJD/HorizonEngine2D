namespace Hzn
{
    public struct Vector4
    {
        public float X, Y, Z, W;

        public static Vector4 Zero => new Vector4(0.0f);

        public Vector4(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
            W = scalar;
        }

        public Vector4(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public Vector4(Vector3 xyz, float w)
        {
            X = xyz.X;
            Y = xyz.Y;
            Z = xyz.Z;
            W = w;
        }

        public Vector3 XY
        {
            get => new Vector3(X, Y, Z);
            set
            {
                X = value.X;
                Y = value.Y;
                Z = value.Z;
            }
        }

        public override bool Equals(object obj)
        {
            if (obj.GetType() != GetType()) return false;
            Vector4 b = (Vector4)obj;
            return (X - b.X) < 1e-6f && (Y - b.Y) < 1e-6f && (Z - b.Z) < 1e-6f && (W - b.W) < 1e-6f;
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public static Vector4 operator +(Vector4 a, Vector4 b)
        {
            return new Vector4(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);
        }

        public static Vector4 operator *(Vector4 vector, float scalar)
        {
            return new Vector4(vector.X * scalar, vector.Y * scalar, vector.Z * scalar, vector.W * scalar);
        }

        public static bool operator ==(Vector4 a, Vector4 b)
        {
            return (a.X - b.X) < 1e-6f && (a.Y - b.Y) < 1e-6f && (a.Z - b.Z) < 1e-6f && (a.W - b.W) < 1e-6f;
        }

        public static bool operator !=(Vector4 a, Vector4 b)
        {
            return !(a == b);
        }
    }
}