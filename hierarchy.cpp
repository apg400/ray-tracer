#include <algorithm>
#include <queue>
#include "hierarchy.h"

// Reorder the entries vector so that adjacent entries tend to be nearby.
void Hierarchy::Reorder_Entries() {
    if(!entries.size()) return;
    std::sort(entries.begin(), entries.end());
}

// Populate tree from entries.
void Hierarchy::Build_Tree() {
    if(!entries.size()) return;
    tree.resize(2 * entries.size() - 1);  
    int index = entries.size()-1;
    
    for (int i = tree.size()-1; i >=0; i--) {
        if (index >= 0) {
            tree[i] = entries[index].box;
            index--;
        } else {
            tree[i] = tree[(2*i)+1].Union(tree[(2*i)+2]);
        }
    }
}

// Return a list of candidates (indices into the entries list) whose
// bounding boxes intersect the ray.
void Hierarchy::Intersection_Candidates(const Ray& ray, std::vector<int>& candidates) const {
    std::queue<int> index_to_traverse;
    index_to_traverse.push(0);
    size_t tree_size = tree.size();
    size_t entry_size = entries.size();

    while (!index_to_traverse.empty()) {
        int index = index_to_traverse.front();
        index_to_traverse.pop();
        
        if (index >= tree_size) continue;
        if (tree[index].Intersection(ray)) {
            // tree[entry_size-1] is the first leaf node 
            if (index >= entry_size-1) {
                candidates.push_back(index - (entry_size-1));
            } else {
                //left = 2*index + 1, right = 2*index + 2
                index_to_traverse.push(2*index + 1);
                index_to_traverse.push(2*index + 2);
            }
        }
    }
}

