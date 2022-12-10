namespace Hzn
{
    public class Scene
    {
        internal Scene(string name)
        {
            Name = name;
        }

        public readonly string Name;

        public GameObject CreateGameObject(string name)
        {
            uint ObjectID = InternalCalls.Scene_CreateGameObject(name);
            return new GameObject(ObjectID);
        }

        public GameObject GetGameObjectByID(uint ID)
        {
            uint gameObjectID = InternalCalls.Scene_GetGameObjectByID(ID);
            if (gameObjectID == System.UInt32.MaxValue)
            {
                return null;
            }
            return new GameObject(ID);
        }

        public bool DestroyGameObject(ref GameObject obj)
        {
            if (obj != null)
            {
                InternalCalls.Scene_DestroyGameObject(obj.ID);
                obj = null;
                return true;
            }
            return false;
        }
    }
}