import sys
import numpy as np
from teca_py_data import *
from teca_py_io import *
from teca_py_alg import *

t1 = teca_table.New()
t1.declare_columns(['event','day','strength','magnitude'], ['i','s','f','d'])
t1.declare_column('flag', 'ull')

c1 = np.array([1,2,3,4,5], dtype=np.int)
c2 = ['Mon', 'Tues', 'Wed', 'Thurs', 'Fri']
c3 = np.array([1.2,2.3,3.4,4.5,5.6], dtype=np.double)
c4 = np.array([6,7,8,9,10], dtype=np.double)
c6 = np.array([0,1,0,1,0], dtype=np.int)

t1.set_column('event', np.array([1,2,3,4,5], dtype=np.int))
t1.set_column('day', ['Mon', 'Tues', 'Wed', 'Thurs', 'Fri'])
t1.set_column('strength', np.array([1.2,2.3,3.4,4.5,5.6], dtype=np.double))
t1.set_column('magnitude', np.array([6,7,8,9,10], dtype=np.double))
t1.set_column('flag', np.array([0,1,0,1,0], dtype=np.int))

sys.stderr.write('dumping table contents...\n')
sys.stderr.write('%s\n'%(str(t1)))

t1[2,1] = 'Sat'
t1[3,1] = 'Sun'

sys.stderr.write('dumping column 1...\n')
r = 0
c = 1
while r < 5:
    sys.stderr.write('t1[%d,%d] = %s\n'%(r,c,str(t1[r,c])))
    r += 1

t2 = teca_table.New()
t2.declare_column('summary', 's')
t2.set_column('summary', ['a','b','c','d','e','f'])

sys.stderr.write('dumping table contents...\n')
sys.stderr.write('%s\n'%(str(t2)))

sys.stderr.write('packaging tables into a workbook...\n')
wbk = teca_workbook.New()
wbk.append_table('summary', t2)
wbk.append_table('details', t1)

sys.stderr.write('dumping the workbook...\n')
sys.stderr.write('%s\n'%(str(wbk)))


sys.stderr.write('writing table to disk...\n')
def serve_table(port, data, req):
    global t1
    return t1

tab_serv = teca_programmable_algorithm.New()
tab_serv.set_number_of_input_connections(0)
tab_serv.set_execute_callback(serve_table)

tab_wri = teca_table_writer.New()
tab_wri.set_input_connection(tab_serv.get_output_port())
tab_wri.set_file_name('table_%t%.%e%')
tab_wri.update()


sys.stderr.write('writing workbook to disk...\n')
def serve_workbook(port, data, req):
    global wbk
    return wbk

wbk_serv = teca_programmable_algorithm.New()
wbk_serv.set_number_of_input_connections(0)
wbk_serv.set_execute_callback(serve_workbook)

wbk_wri = teca_table_writer.New()
wbk_wri.set_input_connection(wbk_serv.get_output_port())
wbk_wri.set_file_name('workbook_%s%_%t%.%e%')
wbk_wri.update()
