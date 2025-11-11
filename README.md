# Options Pricing
## Reverse Mode AD Framework

 I will be storing the nodes from the computational graph in a vector. They will be inserted as the operations are completed in the correct order, resulting in a topological sort of the computation graph. A reverse path can easily be executed by traversing the tape (the vector).