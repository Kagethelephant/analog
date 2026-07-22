
// Create BVH ------------------------------
// order the triangles along the longest axis
// split those in half, split those in half ....
// until you have the desired number of triangles
// create a BVH node for each group and create a convex hull
//
//
// Create convex hull ------------------------------
// take all the bvh nodes and generate a tet (as large as posible)
// cycle through all points and assign them to faces that are facing them (cull ones inside tet)
// cycle through faces and calculate distances to points in faces queue to find farthest point (cull ones inside poly)
// cycle through the remaining faces and remove the faces facing the point
// keep track of the faces edges, and catalog the edges and remove the ones that have duplicates
// use the remaining edges to make new faces with the point
// reasign points of culled face to all new faces
// repeat until there are no points outside of the hull
