from flask import Flask, request, jsonify
import openpyxl
import json
import datetime

book = openpyxl.load_workbook('data.xlsx')
sheet= book.active

sheet['A1'] = 'N'
sheet['B1'] = 'User ID'
sheet['C1'] = 'Datetime'
sheet['D1'] = 'Item'
sheet['E1'] = 'Price'

app = Flask(__name__)
 
@app.route('/', methods=['POST', 'GET'])
def index():
    if request.method == 'POST':
        global lastIndex, buffer
        print("Пришел POST запрос")
        data = request.json
        time = datetime.datetime.now().time()
        data = request.get_json()

        check = data['check']
        user_id = data['user_id']
        row = 2    
        i = 1
        for product in check:
            item = {}
            
            item['N'] = lastIndex
            item['userId'] = user_id
            item['datetime'] = time
            item['item'] = product['item']
            item['price'] = product['price']

            lastIndex += 1

            buffer.append(item);

        

        #дальше проверяем сколько в буфере, если 1000 или больше - пишем в эксель
        if len(buffer) > 1000:
            for product in buffer:
                sheet[row][0].value = product['N']
                sheet[row][1].value = product['userId']
                sheet[row][2].value = product['datetime']
                sheet[row][3].value = product['item']
                sheet[row][4].value = product['price']
                row += 1
                
            lastIndex = 1
            row = 2
            buffer.clear()
        book.save('data.xlsx')
        book.close()
        
        print(buffer)

        return 'OK'
        #return jsonify(data)
 
if __name__ == "__main__":
    global lastIndex, buffer
    
    lastIndex = 1
    buffer = [];

    app.run()
