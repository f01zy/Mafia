import type { Server } from "socket.io";
import { logger } from "../utils/logger.utils";

export const loadIoListeners = (io: Server) => {
  io.on("connection", (socket) => {
    logger.info(`User ${socket.id} was connected`)
  })
}
