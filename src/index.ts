import dotenv from "dotenv"
import { Variables } from "./env/variables.env"

const i = process.argv.indexOf("--mode")
const mode = process.argv[i + 1]
const env = i == -1 ? ".env" : `.${mode}.env`

dotenv.config({
  path: env
})
Variables.initialize()

import { createServer } from "node:http"
import { Server } from "socket.io"
import errorMiddleware from "./middlewares/error.middleware"
import { router } from "./routers"
import { loadIoListeners } from "./socket"
import { prisma } from "./prisma"
import cors from "cors"
import cookieParser from "cookie-parser"
import { logger } from "./utils/logger.utils"
import express from "express"

const app = express()
const server = createServer(app)
const io = new Server(server)

loadIoListeners(io)

app.use(express.urlencoded({ extended: true }))
app.use(express.json())
app.use(cookieParser())
app.use(cors({ credentials: true }))
app.use("/api", router)
app.use(errorMiddleware as any)

const PORT = process.env.PORT || 3000

const start = async () => {
  try {
    await prisma.$connect().then(() => logger.info("PostgreSQL connected"))
    server.listen(PORT, () => {
      logger.info(`Server running at ${mode} mode`)
    })
  } catch (err) {
    console.log(err)
    await prisma.$disconnect()
  }
}

start()
