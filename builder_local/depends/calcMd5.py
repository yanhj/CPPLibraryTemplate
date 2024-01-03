import hashlib
import os
import json

class FileNode:
    def __init__(self, relative_path, md5):
        self.relative_path = relative_path
        self.md5 = md5

    def to_dict(self):
            # 处理 FileNode 类的对象，返回可序列化的形式
        return {"relative_path": self.relative_path,
                "md5": self.md5
                }
  
#自定义 json 序列化格式
def custom_json_encoder(obj):
    print(obj)
    #判断是否是自定义的列表
    if isinstance(obj, FileNode):
        # 自定义字典序列化格式
        return obj.to_dict()
    
    raise TypeError("Object of type {} is not JSON serializable".format(type(obj).__name__))
      
#定义 MD5 计算类
class CalcMd5:
    def __init__(self):
        pass

    #计算字符串的 MD5 值
    def calcStr(self, str):
        md5 = hashlib.md5()
        md5.update(str.encode('utf-8'))
        return md5.hexdigest()
    
    #计算文件的 MD5 值
    def calc(self, file):
        md5 = hashlib.md5()
        with open(file, 'rb') as f:
            while True:
                data = f.read(1024)
                if data:
                    md5.update(data)
                else:
                    break
        return md5.hexdigest()
    
    #计算文件夹的 MD5 值,并返回各个文件的相对路径和 MD5 值
    def calcDir(self, dir):
        md5Node = []
        for root, dirs, files in os.walk(dir):
            for file in files:
                md5 = self.calc(os.path.join(root, file))
                #把文件的相对路径和 MD5 值存入列表
                md5Node.append(FileNode(os.path.join(root, file).replace(dir, ''), md5))
                
        return md5Node
    
#main函数
if __name__ == '__main__':
    #解析参数
    import argparse
    parser = argparse.ArgumentParser(description='计算文件或文件夹的 MD5 值')
    parser.add_argument('-f', '--file', help='文件路径')
    parser.add_argument('-d', '--dir', help='文件夹路径')
    parser.add_argument('-o', '--output', help='输出计算结果到指定文件')
    
    args = parser.parse_args()
    #判断参数是否正确
    if args.file == None and args.dir == None:
        print('参数错误')
        #打印帮助信息
        parser.print_help()
        exit(1)
    #获取路径
    path = ''
    if args.file != None:
        path = args.file
    else:
        path = args.dir
    #判断路径是否存在
    if not os.path.exists(path):
        print('路径不存在')
        exit(1)
    #判断是文件还是文件夹
    calc = CalcMd5()
    if os.path.isfile(path):
        #计算文件的 MD5 值
        md5 = calc.calc(path)
        #写入文件
        if args.output != None:
            with open(args.output, 'w') as f:
                f.write(md5)
                fileHash = []
                #获取文件名
                fileHash.append(FileNode(os.path(path).filename, md5))
                jsondumps_data = json.dumps(fileHash, default=custom_json_encoder, indent=4)
                config_dict={"vesion": "1.0.0",
                             "files": json.loads(jsondumps_data)
                             }
                #把字典序列化为 json 格式
                jsondumps_data = json.dumps(config_dict, indent=4)
                f.write(jsondumps_data)
        else:
            print(md5)
    else:
        #计算文件夹的 MD5 值,并返回各个文件的相对路径和 MD5 值
        fileHash = calc.calcDir(path)
        #把列表序列化为 json 格式
        jsondumps_data = json.dumps(fileHash, default=custom_json_encoder, indent=4)
        config_dict={"vesion": "1.0.0",
                        "files": json.loads(jsondumps_data)
                        }
        #把字典序列化为 json 格式
        jsondumps_data = json.dumps(config_dict, indent=4)
        if args.output != None:
            #写入文件,json格式
            with open(args.output, 'w') as f:
                #保存到文件
                f.write(jsondumps_data)
        else:
            #打印到控制台
            print(jsondumps_data)
        
    

