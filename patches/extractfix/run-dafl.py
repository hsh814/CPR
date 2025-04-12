import subprocess
import multiprocessing as mp
import sys
import os
from typing import Dict, List

CORRECT_PATCHES={
    # 'binutils/CVE-2017-15025',
    # 'binutils/CVE-2018-10372',
    # 'coreutils/bugzilla-19784',
    # 'coreutils/bugzilla-26545',
    'coreutils/gnubug-25003':389,
    'coreutils/gnubug-25023':128,
    'jasper/CVE-2016-8691':260,
    # 'jasper/CVE-2016-9387',
    'libjpeg/CVE-2012-2806':261,
    'libjpeg/CVE-2017-15232':2195,
    # 'libjpeg/CVE-2018-14498',
    # 'libjpeg/CVE-2018-19664',
    # 'libtiff/bugzilla-2611',
    'libtiff/bugzilla-2633':131,
    # 'libtiff/CVE-2014-8128',
    # 'libtiff/CVE-2016-3186',
    # 'libtiff/CVE-2016-3623',
    # 'libtiff/CVE-2016-5314',
    'libtiff/CVE-2016-5321':175,
    # 'libtiff/CVE-2016-9273',
    'libtiff/CVE-2016-10094':257,
    # 'libtiff/CVE-2017-7595',
    'libtiff/CVE-2017-7601':188,
    'libxml2/CVE-2012-5134':261,
    # 'libxml2/CVE-2016-1834',
    'libxml2/CVE-2016-1838':389,
    # 'libxml2/CVE-2016-1839',
    # 'libxml2/CVE-2017-5969',
}

def run(sub:str):
    try:
        log_file=open(f'{sub}/dafl-test.log', 'w')
        inputs=os.listdir(f'{sub}/concrete-inputs')
        if 'coreutils' in sub:
            cmd='./dafl-patched/bin < <exploit> '
        else:
            cmd='./dafl-patched/bin '
            if os.path.exists(f'{sub}/config'):
                with open(f'{sub}/config','r') as f:
                    for line in f:
                        if line.startswith('cmd='):
                            cmd+=' '.join(line[4:].strip().split())
            else:
                with open(f'{sub}/repair.conf','r') as f:
                    for line in f:
                        if line.startswith('test_input_list:'):
                            cmd+=' '.join(line[17:].strip().replace('$POC','<exploit>').split())

        # Run original program with inputs
        print(f'Running {sub} with {len(inputs)} inputs...')
        orig_returncode:Dict[str, int]=dict()
        orig_conditions:Dict[str, List[int]]=dict()
        env=os.environ.copy()
        env['LD_LIBRARY_PATH']=f'{os.getcwd()}/{sub}/dafl-src'
        env['DAFL_PATCH_ID']='0'
        env['DAFL_RESULT_FILE']=f'{os.getcwd()}/{sub}/dafl-condition.log'
        for input in inputs:
            input_path=os.path.join(os.getcwd(),sub,'concrete-inputs', input)
            cur_cmd=cmd.replace('<exploit>', input_path)

            print(cur_cmd,file=log_file)
            res=subprocess.run(cur_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=sub, env=env,shell=True)
            print(f'{input} returns {res.returncode} with patch 0',file=log_file)
            orig_returncode[input]=res.returncode
            if res.returncode != 0 and res.returncode != 1: # If return code is 1, it is not a vulnerability
                print(f'Program crashed with input {input}',file=log_file)
            else:
                print(f'Program executed successfully with input {input}',file=log_file)
            try:
                print(res.stderr.decode('utf-8'),file=log_file)
            except UnicodeDecodeError:
                print('Error decoding stderr',file=log_file)

            # Parse the condition at the location
            if os.path.exists(f'{os.getcwd()}/{sub}/dafl-condition.log'):
                with open(f'{os.getcwd()}/{sub}/dafl-condition.log', 'r') as f:
                    line=f.readline()
                    orig_conditions[input]=[int(x) for x in line.strip().split()]
                os.remove(f'{os.getcwd()}/{sub}/dafl-condition.log')
                print(f'Successfully parse original condition log for {input}: {orig_conditions[input]}',file=log_file)
            else:
                print(f'No original condition log for {input}',file=log_file)
                orig_conditions[input]=[]
            
        # Run patched program with non-crashing inputs and compare branches, filter out if the patch crashes or covers different branches
        for input in inputs:
            if orig_returncode[input] != 0:
                continue

        # Run patched program with crashing inputs, filter out if the patch still crashes
        log_file.close()
        print(f'{sub} finished')
    except Exception as e:
        import traceback
        traceback.print_exc(file=log_file)
        traceback.print_tb(e.__traceback__, file=log_file)
        log_file.close()
        print(f'Error running {sub}: {e}')

pool=mp.Pool(int(sys.argv[1]))
for sub in CORRECT_PATCHES.keys():
    pool.apply_async(run, args=(sub,))
pool.close()
pool.join()