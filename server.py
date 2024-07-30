from flask import Flask, request, jsonify

app = Flask(__name__)

# To store data from POST method
list_data = []

@app.route('/')
def entry_point():
    return 'Hello World! Ini Umy yang lagi kejar materi huft'

@app.route('/sensor/data', methods=['POST'])
def post_data():
    # Get the data from the request
    data = request.json  # Use request.json to get the JSON data
    temperature = request.args.get("temperature")
    humidity = request.args.get("kelembapan")

    # Store the sensor data
    list_data.append({'temperature': temperature, 'kelembapan': humidity})

    # Response message
    response = {'message': 'Data received successfully!'}

    # Return the response
    return jsonify(response)

@app.route('/sensor/data', methods=['GET'])
def get_data():
    # Return the sensor data
    return jsonify(list_data)

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
