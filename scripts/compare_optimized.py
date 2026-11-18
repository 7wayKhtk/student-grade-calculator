"""Paired fresh-process measurements of the selected list/strategy-2 combination."""
import csv,pathlib,subprocess,sys,tempfile
exe=str(pathlib.Path(sys.argv[1]).resolve());output=pathlib.Path(sys.argv[2]).resolve()
with tempfile.TemporaryDirectory(prefix='student-paired-') as d, output.open('w',newline='') as out:
 work=pathlib.Path(d);writer=None
 for n in [1000,10000,100000,1000000,10000000]:
  subprocess.run([exe,'--generate',str(n),'input.txt'],cwd=work,check=True)
  for run in [1,2,3]:
   for impl in (['baseline','algorithms'] if run%2 else ['algorithms','baseline']):
    r=subprocess.run([exe,'--run','input.txt','list','2','1','1',impl,str(run),'one.csv'],cwd=work,capture_output=True,text=True)
    if r.returncode: raise RuntimeError(r.stderr)
    with (work/'one.csv').open() as f:row=next(csv.DictReader(f))
    assert int(row['students'])==n and int(row['passed'])==n*4//10
    row['file']=f'students{n}.txt'
    if writer is None:writer=csv.DictWriter(out,fieldnames=row.keys());writer.writeheader()
    writer.writerow(row);out.flush()
    print(n,run,r.stdout.strip(),flush=True)
    for file in work.glob('*_s*.txt'):file.unlink()
