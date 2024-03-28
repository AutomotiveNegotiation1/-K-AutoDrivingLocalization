cmake_minimum_required(VERSION 3.12)
project(ConvertPythonScript VERSION 1.0.0)

# Python 인터프리터 찾기
find_package(PythonInterp REQUIRED)

# PyInstaller를 사용하여 Python 파일을 실행 파일로 변환하는 사용자 정의 대상
add_custom_target(
    ConvertToExecutable ALL
    COMMAND ${PYTHON_EXECUTABLE} -m PyInstaller --onefile ${CMAKE_SOURCE_DIR}/KETIApp.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Converting your_script.py to an executable with PyInstaller."
)
