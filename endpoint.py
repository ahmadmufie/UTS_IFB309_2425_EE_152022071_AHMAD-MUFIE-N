from flask import Flask, jsonify
from flask_cors import CORS

app = Flask(__name__)

# Enable CORS setelah app didefinisikan
CORS(app)

@app.route('/get_data', methods=['GET'])
def get_data():
    data = {
        "suhumax": 36,
        "suhumin": 21,
        "suhurata": [28, 35],
        "nilai_suhu_max_humid_max": [
            {
                "idx": 101,
                "suhun": 36,
                "humd": 36,
                "kecerahan": 25,
                "timestamp": "2010-09-18 07:23:48"
            },
            {
                "idx": 226,
                "suhun": 36,
                "humd": 36,
                "kecerahan": 27,
                "timestamp": "2011-05-02 17:29:34"
            }
        ],
        "month_year_max": [
            {
                "month_year": "9-2010"
            },
            {
                "month_year": "5-2011"
            }
        ]
    }
    return jsonify(data)

if __name__ == '__main__':
    app.run(debug=True)
