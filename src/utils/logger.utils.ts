import winston from "winston"
import { formatTimestamp } from "./formatTimestamp.ts"

export const logger = winston.createLogger({
  level: "info",
  format: winston.format.combine(
    winston.format.timestamp(),
    winston.format.printf(({ timestamp, level, message }) => {
      const time = formatTimestamp(timestamp as string)
      return `[${level.toUpperCase()}][${time}] ${message}`
    })
  ),
  transports: [
    new winston.transports.Console(),
    new winston.transports.File({ filename: "combined.log" })
  ],
})
