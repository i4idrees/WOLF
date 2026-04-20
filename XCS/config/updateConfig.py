import os, json, re

base_dir = os.path.dirname(os.path.abspath(__file__))
config_path = os.path.join(base_dir, "env_config.json")
header_path = os.path.join(base_dir, "../src/configuration.h")

# Load JSON config
with open(config_path, "r") as f:
    cfg = json.load(f)

# Generate the new config section
new_config = []
new_config.append(f"inline const int num_transactions = {cfg['num_transactions']};")
new_config.append(f"inline Transaction transactions[num_transactions] = {{")
for t in cfg['transactions']:
    new_config.append(f'    {{"{t["name"]}", {t["workLoad"]}}},')
new_config.append("};\n")

new_config.append(f"inline const unsigned int noOfObservations = {cfg['noOfObservations']};")
new_config.append("inline float observations[noOfObservations];\n")

new_config.append(f"const int numActions = {cfg['numActions']};")
new_config.append(f"inline int allPossibleActions[numActions] = {{")
for a in cfg['allPossibleActions']:
    new_config.append(f"    {a},")
new_config.append("};\n")

new_config.append(f"const int maxPopSize = {cfg['maxPopSize']};")
new_config.append(f"const int condLength = {cfg['condLength']};")
new_config.append(f"const double maxPayoff = {cfg['maxPayoff']};\n")

new_config_str = "\n".join(new_config)

# Read original header file
with open(header_path, "r") as f:
    header_content = f.read()

# Replace between markers
pattern = re.compile(r"// =================== CONFIG START ===================.*?// =================== CONFIG END ===================", re.S)
updated_content = re.sub(pattern, f"// =================== CONFIG START ===================\n{new_config_str}\n// =================== CONFIG END ===================", header_content)

# Write back to header file
with open(header_path, "w") as f:
    f.write(updated_content)

print("configuration.h updated successfully!")
