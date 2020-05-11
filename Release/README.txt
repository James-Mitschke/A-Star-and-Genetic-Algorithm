Place any map files you have generated in the "Map Files" folder
Make sure the map files you are using are encoded with "UTF_8"
Double Click on the ReadingAMap.exe to run the program
When asked for a file name, input the name from the maps folder EXACTLY AS IT APPEARS
(This includes the extension e.g, "TestMazeA_UTF_8.txt"

When asked which program you would like to run you have two options that will run the code;
"A*" or "GA"

Due to the fact GA isn't working for a currently undetermined reason at the time of writing this please only use the "A*" program.

The A* program will find and generate one of the shortest possible paths to the end node from the start node if possible and from there output the path via blue tiles. The program will then give you 10 seconds to observe the path taken before shutting itself and needing to be re-run for subsequent files.

NOTE:
Sometimes the A* algorithm path can appear as if it teleports over a tile, this is due to the parent x and y positions being overwritten when they aren't even being accessed. The reason this currently happens is unknown however if you observe the code step by step you'll see the path generated is actually still one of the shortest paths. 

(This can be observed on "TestMazeC_UTF_8.txt" where the path leads from the start node to 1,7 on the map, and then jumps to position 3,8 on the map. What the path should actually do and does do before the parent is overwritten is go from 1,6 > 2,7 > 3,8)