"""Reproduce isolated measurements; run from any directory with Python 3."""
import argparse, csv, pathlib, subprocess, tempfile, json, platform, time
p = argparse.ArgumentParser()
p.add_argument('--exe', required=True)
p.add_argument('--output', required=True)
p.add_argument('--sizes', nargs='+', type=int, default=[1000,10000,100000,1000000,10000000])
p.add_argument('--repetitions', type=int, default=1)
p.add_argument('--implementation', default='baseline', choices=['baseline','algorithms'])
p.add_argument('--container', choices=['vector','list','deque'])
p.add_argument('--strategy', type=int, choices=[1,2])
a=p.parse_args()
exe=str(pathlib.Path(a.exe).resolve()); output=pathlib.Path(a.output).resolve()
output.parent.mkdir(parents=True,exist_ok=True)
metadata={'platform':platform.platform(),'processor':platform.processor(),'sizes':a.sizes,'repetitions':a.repetitions,'implementation':a.implementation,'sort':'name','grade':'average','dataset':'i=1..N, six grades (i+j)%10+1, j=0..5','cache':'not cleared','process':'fresh process for every case','compiler':'g++ -std=c++17 -O2 -Wall -Wextra -pedantic','date':time.strftime('%Y-%m-%d')}
output.with_suffix('.json').write_text(json.dumps(metadata,indent=2)+'\n')
with tempfile.TemporaryDirectory(prefix='student-benchmark-') as d, output.open('w',newline='') as out:
    work=pathlib.Path(d); writer=None
    for n in a.sizes:
        subprocess.run([exe,'--generate',str(n),'input.txt'],cwd=work,check=True)
        pairs=[(c,s) for c in ([a.container] if a.container else ['vector','list','deque']) for s in ([a.strategy] if a.strategy else [1,2])]
        for run in range(1,a.repetitions+1):
            order=pairs[run-1:]+pairs[:run-1]
            for c,s in order:
                command=[exe,'--run','input.txt',c,str(s),'1','1',a.implementation,str(run),'one.csv']
                completed=subprocess.run(command,cwd=work,text=True,capture_output=True)
                if completed.returncode: raise RuntimeError(completed.stderr or completed.stdout)
                with (work/'one.csv').open() as f: row=next(csv.DictReader(f))
                assert int(row['students'])==n
                assert int(row['passed'])+int(row['failed'])==n
                row['file']=f'students{n}.txt'
                if writer is None: writer=csv.DictWriter(out,fieldnames=row.keys());writer.writeheader()
                writer.writerow(row);out.flush()
                print(n,run,completed.stdout.strip(),flush=True)
                # Delete generated groups between cases; retain measured results only.
                for file in work.glob('*_s*.txt'): file.unlink()
