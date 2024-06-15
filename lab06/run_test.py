import os
import argparse
import time
"""
Exemplo de execução do Script:

python run_test.py aux/out/saidas_programa/ aux/out/saidas_corretas
"""
FLAG = True

def get_args():
  parser = argparse.ArgumentParser(
      prog="Programa para testar códigos C",
      description="Considerando dois arquivos de texto, mostrar a ocorrência de diferença entre ambos."
  )
  parser.add_argument("saidas_programa_diretorio", help="Pasta com os arquivos .out da saída do seu programa")
  parser.add_argument("saidas_corretas_diretorio", help="Pasta com os arquivos .out das saídas esperadas(corretas)")
  args = parser.parse_args()

  return args

def compare_files(file1, file2):
    global FLAG

    with open(file1, 'r') as f1, open(file2, 'r') as f2:
        lines1 = f1.readlines()
        lines2 = f2.readlines()

    for i, (line1, line2) in enumerate(zip(lines1, lines2), start=1):
        if line1 != line2:
            print("\n<><><><><><><><><><><><><><><><><><><><><><><><><>\n")
            print(f"[INFO] Linha {i} diferente entre os dois arquivos. \n")
            print(f"Saida do programa: {file1}: {line1.strip()}")
            print(f"Saida correta: {file2}: {line2.strip()}")
            print("\n<><><><><><><><><><><><><><><><><><><><><><><><><>\n")
            print("--------------------------------------------------")
            FLAG = False
"""
def add_newline(file):
    with open(file, 'a') as f:
        f.write('\n')
   

def remove_last_line(filename):
    with open(filename, 'r+') as file:
        lines = file.readlines()
        if lines and lines[-1] == "\n":
            lines = lines[:-1]
        file.seek(0)
        file.writelines(lines)
        file.truncate()

"""
if __name__ == "__main__":
  args = get_args()
  inputs_dir = os.listdir(args.saidas_programa_diretorio)
  outputs_dir = os.listdir(args.saidas_corretas_diretorio)
  #for file1,file2 in zip(inputs_dir, outputs_dir):
 #   remove_newlineV2(os.path.join(args.saidas_programa_diretorio,file1))
  #time.sleep(3)
  for file1,file2 in zip(inputs_dir, outputs_dir):
    # Call the function with the paths to your files
    compare_files(os.path.join(args.saidas_programa_diretorio,file1), 
                  os.path.join(args.saidas_corretas_diretorio,file2))
    #add_newline(os.path.join(args.saidas_programa_diretorio,file1))

    
  if(FLAG):
     print("\n[INFO] :) TUDO OK !!! \n")
