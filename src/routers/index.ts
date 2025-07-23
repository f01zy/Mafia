import { Router } from "express";
import { authRouter } from "./user.router";

export const router = Router()

router.use("/auth", authRouter)
