from flask import Flask, request, jsonify
from flask_cors import CORS  # 🧩 Import CORS
import mysql.connector

app = Flask(__name__)
CORS(app)  # ✅ Enable CORS for all routes

# 🗄️ MySQL config — update with your details!
db_config = {
    'host': 'localhost',
    'user': 'root',
    'password': 'K1J[8]a18D',
    'database': 'sensor_db'
}

# ✅ Route to receive sensor data (POST)
@app.route('/data', methods=['POST'])
def insert_data():
    data = request.get_json()
    temperature = data.get('temperature')
    humidity = data.get('humidity')

    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor()
    cursor.execute(
        "INSERT INTO sensor_data (temperature, humidity) VALUES (%s, %s)",
        (temperature, humidity)
    )
    conn.commit()
    cursor.close()
    conn.close()

    return jsonify({'message': 'Data inserted successfully'})

# ✅ Route to fetch all sensor data (GET)
@app.route('/data', methods=['GET'])
def get_all_data():
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM sensor_data")
    rows = cursor.fetchall()
    cursor.close()
    conn.close()

    result = []
    for row in rows:
        result.append({
            'id': row[0],
            'temperature': row[1],
            'humidity': row[2],
            'timestamp': str(row[3])
        })

    return jsonify(result)

# ✅ Test route to check if server is up
@app.route('/', methods=['GET'])
def home():
    return "ESP32 Flask API is running!"

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000, debug=True)
