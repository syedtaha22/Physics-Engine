# Simulation CSV Format 

This document provides guidelines for formatting CSV files to be used with the simulation program.

## CSV Structure

The CSV file must have the following structure:

### Header Row

The first row in the CSV file can either:
- Contain a descriptive header (e.g., `mass,distanceX,distanceY,velocityX,velocityY`), or  
- Be left empty.  

### Data Rows

Each subsequent row should represent an astronomical body with the following properties:
1. **Mass** (`mass`): The mass of the body in kilograms (e.g., `1.9885e30` for the Sun).
2. **DistanceX** (`distanceX`): The X-coordinate of the body's distance from the origin in meters.
3. **DistanceY** (`distanceY`): The Y-coordinate of the body's distance from the origin in meters.
4. **VelocityX** (`velocityX`): The X-component of the body's velocity in meters per second.
5. **VelocityY** (`velocityY`): The Y-component of the body's velocity in meters per second.

### Example CSV

Below is an example of a properly formatted CSV file:

```csv
mass,distanceX,distanceY,velocityX,velocityY
1.9885e30,0,0,0,0
3.3011e23,5.79e10,0,0,47870
4.8675e24,1.082e11,0,0,35020
5.9724e24,1.496e11,0,0,29780
6.4171e23,2.279e11,0,0,24070
1.8982e27,7.785e11,0,0,13070
5.6834e26,1.4335e12,0,0,9690
8.6810e25,2.8725e12,0,0,6810
1.0241e26,4.4951e12,0,0,5430
```

Alternatively, the header can be empty:

```csv
,,,
1.9885e30,0,0,0,0
3.3011e23,5.79e10,0,0,47870
4.8675e24,1.082e11,0,0,35020
5.9724e24,1.496e11,0,0,29780
6.4171e23,2.279e11,0,0,24070
1.8982e27,7.785e11,0,0,13070
5.6834e26,1.4335e12,0,0,9690
8.6810e25,2.8725e12,0,0,6810
1.0241e26,4.4951e12,0,0,5430
```

### Notes

- **Scientific Notation**: Mass and distance values can be provided in scientific notation (e.g., `1.9885e30`).
- **Units**: Ensure all values are in SI units:
  - Mass: Kilograms (kg)
  - Distance: Meters (m)
  - Velocity: Meters per second (m/s)
- **No Empty Rows**: Avoid empty rows or spaces between data rows.

### Common Errors to Avoid

- **Missing or Extra Columns**: The CSV must strictly follow the `mass,distanceX,distanceY,velocityX,velocityY` format for data rows.
- **Invalid Data**: Ensure all values are numeric and valid (e.g., no letters or special characters in the data fields).