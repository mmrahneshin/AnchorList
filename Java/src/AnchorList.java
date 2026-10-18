
public class AnchorList<T> {

    public AnchorList() {
        mIPPS23RBbt = new IndexedPPS23RedBlackBinaryTree<T, IndexedPPS23RBBinaryTreeNode<T>>();
    }

    public void insert(int idx, T data) throws IllegalAccessException {
        if (idx < 0 || idx > this.size()) {
            throw new IllegalAccessException(
                    "out_of_bound");
        }

        if (mIPPS23RBbt.size() == 0) {
            mIPPS23RBbt.insertRootNode(data);
            return;
        }

        if (idx == mIPPS23RBbt.size()) {
            mIPPS23RBbt.insertLastInOrderNode(data);
            return;
        }

        if (idx == 0) {
            mIPPS23RBbt.insertFirstInOrderNode(data);
            return;
        }

        IndexedPPS23RBBinaryTreeNode<T> node = getNodeWithUpdateLeftSize(idx, 1);
        node.mLeftSize += 1;

        if (!mIPPS23RBbt.hasLeftChild(node)) {
            mIPPS23RBbt.insertLeftChild(node, data);
        } else {
            node = node.mLeftChild;
            while (mIPPS23RBbt.hasRightChild(node)) {
                node = node.mRightChild;
            }
            mIPPS23RBbt.insertRightChild(node, data);
        }
    }

    public T get(int idx) throws IllegalAccessException {
        if (idx < 0 || idx >= this.size()) {
            throw new IllegalAccessException(
                    "out_of_bound");
        }
        IndexedPPS23RBBinaryTreeNode<T> node = getNodeWithUpdateLeftSize(idx, 0);
        return node.mData;
    }

    public void remove(int idx) throws IllegalAccessException {
        if (idx < 0 || idx >= this.size()) {
            throw new IllegalAccessException(
                    "out_of_bound");
        }

        if (idx == mIPPS23RBbt.size() - 1) {
            mIPPS23RBbt.deleteLastInOrderNode();
            return;
        }

        if (idx == 0) {
            mIPPS23RBbt.deleteFirstInOrderNode();
            return;
        }

        IndexedPPS23RBBinaryTreeNode<T> node = getNodeWithUpdateLeftSize(idx, -1);

        mIPPS23RBbt.deleteNode(node);
    }

    public int size() {
        return mIPPS23RBbt.size();
    }

    private IndexedPPS23RBBinaryTreeNode<T> getNodeWithUpdateLeftSize(int idx, int status) {
        IndexedPPS23RBBinaryTreeNode<T> node = mIPPS23RBbt.mInOrderEnd;
        node.mLeftSize += status;

        node = node.mLeftChild;
        int leftOnly = 1;
        int leftSize = node.mLeftSize + mIPPS23RBbt.mGlobalLeftSize;
        while (leftSize != idx) {
            if (idx < leftSize) {
                node.mLeftSize += status;
                node = node.mLeftChild;
            } else {
                leftOnly = 0;
                idx = idx - leftSize - 1;
                node = node.mRightChild;
            }
            leftSize = node.mLeftSize + (mIPPS23RBbt.mGlobalLeftSize * leftOnly);
        }
        return node;
    }

    protected IndexedPPS23RedBlackBinaryTree<T, IndexedPPS23RBBinaryTreeNode<T>> mIPPS23RBbt;
}
