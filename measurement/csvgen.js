const fs = require('fs')
const folders = fs.readdirSync('./').filter(f => f.includes('_vars'))

let headerInitialized = false
const output = []

for (const folder of folders) {
  const files = fs.readdirSync(`./${folder}`).filter(f => f.includes('.json'))
  const testName = folder.replace('_vars', '')

  const measurements = []

  for (const file of files) {
    const fileContent = fs.readFileSync(`./${folder}/${file}`, { encoding: 'utf-8' })
    const parsedFileContent = JSON.parse(fileContent)

    for (const measurement of parsedFileContent) {
      measurements.push(measurement)
    }
  }

  const averageMeasurement = {
    expressionLength: 0,
    createDuration: 0,
    useDuration: 0,
    reductionRate: 0
  }

  if (!headerInitialized) {
    output.push(['variablesCount', ...Object.keys(averageMeasurement)])
    headerInitialized = true
  }

  for (const measurement of measurements) {
    for (const key in averageMeasurement) {
      averageMeasurement[key] += measurement[key]
    }
  }

  for (const key in averageMeasurement) {
    averageMeasurement[key] = (averageMeasurement[key] / measurements.length).toFixed(5)
  }

  output.push([
    testName,
    ...Object.values(averageMeasurement).map(String).map(s => s.replace('.', ','))
  ])
}

fs.writeFileSync('./measurements.csv', output.map(row => row.join(';')).join('\n'), { encoding: 'utf-8' })