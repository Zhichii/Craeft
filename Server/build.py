import os
import sys
def compile(name):
    os.system(f"g++ -c {name}.cpp -std=c++20 -o Build/{name}.o")
others = ["../json/json_reader.o","../json/json_value.o","../json/json_writer.o"]
allll = ["utf8","thing","mod","chunk","main","color"]
if len(sys.argv) < 2:
    pass
elif sys.argv[1] == 'all':
    for i in allll:
        compile(i)
else:
    for i in sys.argv[1:]:
        compile(i)
#../json/build.sh
os.system(f"g++ {' '.join([f'Build/{name}.o' for name in allll]+others)} -o CraeftServer")
os.system("./CraeftServer")
