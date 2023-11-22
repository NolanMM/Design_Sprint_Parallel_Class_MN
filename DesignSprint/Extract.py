import pandas as pd
import re

# Function to extract information from the text


def extract_info(text):
    mode_match = re.search(
        r'Mode: (\w+)\nPattern_Size: (\d+)\nFiles_Name: (\w+\.txt)\nText_Size: (\d+)\nnum_Threads: (\d+)\nTotal_Word: (\d+)\nTotal_time: ([\d.]+) seconds', text)
    if mode_match:
        mode = mode_match.group(1)
        pattern_size = int(mode_match.group(2))
        file_name = mode_match.group(3)
        text_size = int(mode_match.group(4))
        num_threads = int(mode_match.group(5))
        total_words = int(mode_match.group(6))
        total_time = float(mode_match.group(7))

        return {'Mode': mode, 'Pattern_Size': pattern_size, 'Files_Name': file_name, 'Text_Size': text_size, 'num_Threads': num_threads, 'Total_Word': total_words, 'Total_time': total_time}


# Read the text file
with open('results.txt', 'r') as file:
    text = file.read()

# Split the text based on the "Record" delimiter
records = re.split(r'Record', text)

# Extract information from each record
data = [extract_info(record) for record in records if extract_info(record)]

# Create a DataFrame from the extracted data
df = pd.DataFrame(data)

# Print the DataFrame for debugging
print("\nDataFrame:")
print(df)

# Save the DataFrame to an Excel file
df.to_excel('results.xlsx', index=False)
