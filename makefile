dic_test : main.c common.o dic_api.o linear_file_operate.o List.o
	gcc -o EngDic68.exe main.c common.o dic_api.o linear_file_operate.o List.o

dic_api.o : dic_api.c dic_api.h
	gcc -c dic_api.c
linear_file_operate.o : linear_file_operate.c linear_file_operate.h
	gcc -c linear_file_operate.c
List.o : List.c List.h
	gcc -c List.c
common.o : common.c common.h
	gcc -c common.c
clean:
	rm common.o dic_api.o List.o linear_file_operate.o
