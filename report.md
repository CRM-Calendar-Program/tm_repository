1. Excel input

 1. QXlsx, openXlsx 등의 외부 라이브러리를 사용하여 Excel의 .xlsx파일을 직접 c++환경에서 사용하려고 했으나,
QT 혹은 Cmake의 사용, 유료화 등의 문제로 .csv파일 사용으로 전환하였다.
 2. 작업은 visual studio 2022환경에서 project1에 작성되었으며 CSV파일 읽기(reader), 파일내 자료 찾기(finder) 등으로 구성되었다.

c++의 ofstream, ifstream, fstream의 입출력 클래스를 사용했다.

2. 향후 계획
 1. Excel input 과정에서 외부 라이브러리를 활용한 xlsx파일 직접 입출력을 계속하여 도전, 적용 할 것. <--LibXL사용예정  주소:  https://www.libxl.com
 2. 필요한 프로그램 기능을 넣을 것.
