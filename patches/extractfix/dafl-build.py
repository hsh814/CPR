import subprocess

CORRECT_PATCHES=(
    'binutils/cve_2017_15025',
    'binutils/cve_2018_10372',
    'coreutils/bugzilla_19784',
    'coreutils/bugzilla_26545',
    'coreutils/gnubug_25003',
    'coreutils/gnubug_25023',
    'jasper/cve_2016_8691',
    'jasper/cve_2016_9387',
    'libjpeg/cve_2012_2806',
    'libjpeg/cve_2017_15232',
    'libjpeg/cve_2018_14498',
    'libjpeg/cve_2018_19664',
    'libtiff/bugzilla_2611',
    'libtiff/bugzilla_2633',
    'libtiff/cve_2014_8128',
    'libtiff/cve_2016_3186',
    'libtiff/cve_2016_3623',
    'libtiff/cve_2016_5314',
    'libtiff/cve_2016_5321',
    'libtiff/cve_2016_9273',
    'libtiff/cve_2016_10094',
    'libtiff/cve_2017_7595',
    'libtiff/cve_2017_7601',
    'libxml2/cve_2012_5134',
    'libxml2/cve_2016_1834',
    'libxml2/cve_2016_1838',
    'libxml2/cve_2016_1839',
    'libxml2/cve_2017_5969',
)

for sub in CORRECT_PATCHES:
    print(f'Applying {sub}...')
    res=subprocess.run(['./afl-init.sh'], stdout=subprocess.PIPE, stderr=subprocess.STDOUT,text=True, cwd=sub)
    with open(f'{sub}/afl-init.log', 'w') as f:
        f.write(res.stdout)
    if res.returncode != 0:
        print(f'Error applying {sub}!')
        continue
    print(f'Finished applying {sub}.\n')