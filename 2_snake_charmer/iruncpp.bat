@ECHO ON
g++ -o %1.exe %1 -lglut32 -lglu32 -lopengl32
.\%1.exe