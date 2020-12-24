# ShipWreck
Welcome to the ultimate ship navigation software. This program allows you to captain your very own ship in the waters     and change directions. The mapping is depicted in the output box below RIMS and the controlled output is on port B and D.     You can change the direction of your ship, and the speed. You also have to get past obstacles in order to prevent your     ship from wrecking.

Controls:
A0 - turns on ship engine
A1 - moves ship right
A2 - moves ship left
A6 - speeds up propellers
A7 - slows down propellers
B0 - ship is on
B1 - ship is moving right
B2 - ship is moving left
B3 - warning is being handled
B4 - direction is north
B5 - direction is east
B6 - direction is south
B7 - direction is west
D - propeller meter 

Tutorial:
Press A0 to begin
You will see a compass and straight tick marks indicating which direction you’re going in
Press A1 and A2 to mess around with the directions of the mapping
When the direction becomes steep enough, it will update the compass and start from that direction.
B4-B7 displays north, east, south, and west respectfully. 
B0 represents if the ship is on
Press A6 and A7 to mess around with the speed of the ship.
The speed meter will be displayed on port D, max speed is 200, and starting speed is 100.
The terminal will display warnings in the sea. Make sure to change directions so your ship won’t wreck.

Functions:

This is the tree that represents the structure for the path for the ship. This is what builds the tick marks for the mapping of the ship in the output and basically what controls everything. What I did is make a struct with nodes for left, right, a direction switch, and a char variable that stores its tick mark. I assigned the nodes to each other to map out the diagram of what the ship path would look like while messing with the left and right switches. It basically goes through all the nodes and follows the path. Below the tree, you can see the code for the function that develops each node, memory, and mapping of the ship path. 

https://www.youtube.com/watch?v=vEBBIri-gTs&t=247s
