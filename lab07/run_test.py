import os
from pathlib import Path
import argparse
import subprocess
import difflib

"""
Exemplo de execução do Script:

python run_test.py ../sols/main in/ out/
python run_test.py ../sols/main.exe in/ out/ (Para Windows)
"""

def get_args():
  parser = argparse.ArgumentParser(
      prog="Programa para testar códigos C para achar saída de Labirinto",
      description="Testa algoritmo de BackTrack"
  )
  parser.add_argument("main", help="Arquivo executável com código a ser testado")
  parser.add_argument("input_folder", help="Pasta com os arquivos txt do labirinto")
  parser.add_argument("gt_output_folder", help="Pasta com as saídas esperadas (corretas)")
  args = parser.parse_args()

  return args

def run_tests(args):
  test_output = Path("test_output")
  os.makedirs(test_output, exist_ok=True)
  input_files = os.listdir(args.input_folder)
  output_files = os.listdir(args.gt_output_folder)
  input_files.sort(key=lambda x: int(x.replace("arq", "").replace(".in", "")))
  output_files.sort(key=lambda x: int(x.replace("arq", "").replace(".out", "")))

  for file in input_files:
    input_file = Path(args.input_folder) / Path(file)
    test_output_file = test_output / Path(file.replace("in", "out"))
    gt_file = Path(args.gt_output_folder) / Path(file.replace("in", "out"))
    print(f"[INFO] Executando teste para {file} | Saída salva em {test_output_file}")

    # Escreve a saída da execução na pasta de teste
    with open(test_output_file, "w") as file:
      subprocess.call(
        [args.main, f"{input_file}"], stdout=file
      )

    # Compara com arquivo da saída esperada (Ground-truth)
    with open(test_output_file, "r") as file:
      test_content = file.read()
    with open(gt_file, "r") as file:
      gt_content = file.read()

    diff = difflib.unified_diff(
      test_content.split("\n"), gt_content.split("\n")
    )
    for line in diff:
      print(line)

if __name__ == "__main__":
  args = get_args()
  run_tests(args)