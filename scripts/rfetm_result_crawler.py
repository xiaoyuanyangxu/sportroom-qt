# este script consigue todos los calles de sabadell con sus coordinadas...
import os, re, urllib.request, sys, time
import requests
import lxml.html as lh

def download_page(url):
    sys.stderr.write("OPEN:%s\n"%(url))
    #print "OPEN:%s"%(url)
    for i in range(0,10):
        try:
            page = requests.get(url)
            return page.content
        except:
            sys.stderr.write("Error in getting page [%s]\n"%(url))
            raise
            if i > 5:
                return ""
            else:
                sys.stderr.write(".... try it again in 5 seconds\n")
                time.sleep(5)
    
    return ""

def crawler(content):
    doc = lh.fromstring(content)
    tr_elements = doc.xpath('//tr')

    for j in range(2,len(tr_elements)):
        T=tr_elements[j]
        row = []
        r = 0
        for t in T.iterchildren():
            if r<6:
                data = t.text_content() 
                data = data.replace("  "," ")
                if r==2:
                    data = data[:-1]

                try:
                    data = int(data)
                except:
                    pass
                row.append(data)
            else:
                data1 = t.find('b').text_content()
                l = len(data1)
                try:
                    data1 = int(data1)
                except:
                    pass
                row.append(data1)
                data = t.text_content()
                data = data[l:]
                data = data.split("/")
                for d in data:
                    try:
                        d = int(d)
                    except:
                        pass
                    row.append(d)
                
            r += 1
        for i in range(len(row)):
            row[i] = str(row[i])
        print("; ".join(row))

def main():
    try:
        url = sys.argv[1]
    except:
        print("USAGE: %s URL "%(sys.argv[0]))
        print("EXAMPLE: %s http://www.rfetm.es/resultados/view.php?op=rk&liga=Mg==&grupo=0&jornada=0&sexo=M"%(sys.argv[0]))
        sys.exit(-1)
    
    content = download_page(url)
    crawler(content)        

if __name__ == "__main__":
    main()
    