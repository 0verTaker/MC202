import subprocess as sp
import os

COLOR_FAIL = '\033[91m'
COLOR_PASSED = '\033[92m'
COLOR_END = '\033[0m'


def test(exe_path):
    for test_file in os.scandir(f"teste_{exe_path}"):
        if test_file.name.endswith(".in"):
            with open(test_file, 'r') as file:
                lines_to_test = file.readlines()
                test_input(lines_to_test, exe_path, test_file.name)


def test_out(exe_path, test_file):
    out_file_path = f"teste_{exe_path}/{test_file.replace('.in', '.out')}"
    with open(out_file_path, 'r') as file:
        return file.readlines()


def test_input(lines_to_test, exe_path, test_file):
    combined_input = "".join(lines_to_test)
    process = sp.Popen([f".\{exe_path}.exe"], stdin=sp.PIPE, stdout=sp.PIPE, stderr=sp.PIPE, text=True)
    stdout, stderr = process.communicate(input=combined_input, timeout=10)
    
    expected_output_string = "".join(test_out(exe_path, test_file)).strip()
    
    if expected_output_string == stdout.strip():
        print(f"{COLOR_PASSED}Test {test_file}: Successful test{COLOR_END}")
    else:
        print(f"{COLOR_FAIL}Test {test_file}: Failed test", COLOR_END)

def main():
    exe_path = input("Nome do arquivo: ")
    
    try:
        sp.check_output(f"gcc {exe_path}_main.c {exe_path}.c -o {exe_path} -lm", stderr=sp.STDOUT, shell=True)
    except:
        sp.run(f"gcc {exe_path}.c -o {exe_path} -lm", shell=True)
    
    test(exe_path)


if __name__ == "__main__":
    main()
