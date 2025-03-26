## Follow up Questions for Multithreading Web Crawler LC 1242

### 1. Assume we have 10,000 nodes and 1 billion URLs to crawl. We will deploy the same software onto each node. The software can know about all the nodes. We have to minimize communication between machines and make sure each node does equal amount of work. How would your web crawler design change?

- In this case, we would need to distribute the URLs across the nodes in a balanced way. One possible approach is to use a consistent hashing algorithm to assign URLs to nodes. Consistent hashing would ensure that if a node is added or removed, only a minimal number of URLs need to be reassigned. Each node would then independently crawl its assigned URLs. To minimize communication between nodes, each node could maintain its own visited set.

1. Nodes refer to individual machines running your crawler software, each with its own internal multithreading.
2. Consistent hashing: is used to distribute URLs among these nodes. This way, each node receives a subset of URLs to crawl locally.
3. When nodes are added or removed, consistent hashing minimizes the re-distribution of URLs, ensuring most URLs remain assigned to the same machine.

### 2. What if one node fails or does not work?

- If a node fails, the URLs assigned to it would need to be reassigned to other nodes. This could be done by having a master node monitor the health of the worker nodes. If a worker node fails, the master node could use the consistent hashing algorithm to reassign the URLs to the remaining nodes.

### 3. How do you know when the crawler is done?

- The crawler is done when all URLs have been visited. This could be determined by having each node report back to the master node when it has finished crawling its assigned URLs. When the master node has received a completion report from all nodes, it knows that the crawling is done. Alternatively, if each node maintains its own visited set, the master node could periodically query the nodes for their visited sets and check if all URLs have been visited.
