import { Router } from "express"
import { body } from "express-validator"
import { UserController } from "../controllers/user.controller"

const userController = new UserController()

export const authRouter = Router()

authRouter.post("/register",
  body("username").isString().isLength({ min: 4, max: 16 }),
  body("email").isEmail(),
  body("password").isString().isLength({ min: 8, max: 30 }),
  userController.register
)
authRouter.post("/login",
  body("username").isString(),
  body("password").isString(),
  userController.login
)
authRouter.post("/logout", userController.logout)
authRouter.post("/resendActivationCode", userController.resendActivationCode)
authRouter.get("/activate/:code", userController.activate)
authRouter.post("/refresh", userController.refresh)
