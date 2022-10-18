#include <vector>

namespace Hzn {
	template <typename T>
	class TreeNode {
	public:
		int level;
		T item;
		std::vector<std::shared_ptr<TreeNode<T>>> nextNodes;
	public:
		TreeNode<T>();
		TreeNode<T>(int level, T item);
		~TreeNode<T>();

		void add(std::shared_ptr<TreeNode<T>> treeNode);
		void remove(std::shared_ptr<TreeNode<T>> treeNode);
	};
}