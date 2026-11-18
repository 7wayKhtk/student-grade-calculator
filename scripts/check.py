"""Black-box regression checks against independently calculated grades."""
import pathlib,subprocess,tempfile,statistics,itertools,csv,sys
exe=str(pathlib.Path(sys.argv[1]).resolve())
cases=[[], [('Pass','Only',[10],10)], [('Fail','Only',[1],1)],
 [('Boundary','Five',[5],5),('Mean','Median',[1,1,10],5),('No','Homework',[],10),('Even','Grades',[2,8],5),('Zulu','Alpha',[10],10),('Alpha','Zulu',[1],1)]]
with tempfile.TemporaryDirectory() as d:
 p=pathlib.Path(d)
 for records in cases:
  text='\n'.join(f'{name} {surname} {len(hw)} '+ ' '.join(map(str,hw+[exam])) for name,surname,hw,exam in records)
  (p/'input.txt').write_text(text)
  for container,strategy,sort,grade,impl in itertools.product(['vector','list','deque'],[1,2],[1,2],[1,2],['baseline','algorithms']):
   r=subprocess.run([exe,'--run','input.txt',container,str(strategy),str(sort),str(grade),impl,'1','out.csv'],cwd=p,capture_output=True,text=True)
   assert r.returncode==0,r.stderr
   row=next(csv.DictReader((p/'out.csv').open()))
   expected={'passed':[],'failed':[]}
   ordered=sorted(records,key=lambda x:(x[0],x[1]) if sort==1 else (x[1],x[0]))
   for name,surname,hw,exam in ordered:
    result=0.4*((statistics.mean(hw) if grade==1 else statistics.median(hw)) if hw else 0)+0.6*exam
    line=f'{name} {surname} {len(hw)} '+ ' '.join(map(str,hw+[exam]))+f' {result:.2f}'
    expected['passed' if result>=5 else 'failed'].append(line)
   for group in expected:
    actual=(p/f'{group}_{container}_s{strategy}_{impl}.txt').read_text().splitlines()[1:]
    assert actual==expected[group],(group,actual,expected[group])
   assert int(row['retained_records'])==len(records)*(2 if strategy==1 else 1)
 for malformed in ['A B 2 5','A B -1 5','A B 1 11 5','A B 1 5 5 trailing','A B 1 5 0']:
  (p/'bad.txt').write_text(malformed)
  r=subprocess.run([exe,'--run','bad.txt','vector','1','1','1','baseline','1','out.csv'],cwd=p,capture_output=True)
  assert r.returncode==1
 for args in [['--generate','0','a.txt'],['--run','missing.txt','vector','1','1','1','baseline','1','out.csv']]:
  assert subprocess.run([exe]+args,cwd=p,capture_output=True).returncode==1
 assert subprocess.run([exe],input='',text=True,cwd=p,capture_output=True).returncode==1
print('PASS: 192 valid end-to-end cases, grade boundary, both sort/grade modes, both strategies/implementations, malformed/missing input and EOF.')
